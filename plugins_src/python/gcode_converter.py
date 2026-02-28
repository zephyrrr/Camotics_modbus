import math
import re

class GCodeFlattener:
    def __init__(self):
        self.vars = {}
        self.subs = {}
        self.output = []

    def parse_value(self, val):
        val = val.strip()
        # Handle variables like H661 or #1
        if val.startswith('H') or val.startswith('#'):
            return self.vars.get(val.replace('#', 'VAR'), 0)
        try:
            return float(val)
        except:
            return val

    def evaluate_expression(self, expr):
        # Clean up G-code math syntax for Python
        expr = expr.replace('[', '(').replace(']', ')')
        expr = expr.replace('SQRT', 'math.sqrt')
        # Replace H and # variables with their current values
        for var in sorted(self.vars.keys(), key=len, reverse=True):
            expr = expr.replace(var, str(self.vars[var]))
        # Replace any remaining #1 style variables
        expr = re.sub(r'#(\d+)', lambda m: str(self.vars.get('VAR'+m.group(1), 0)), expr)
        
        try:
            return eval(expr, {"math": math})
        except Exception as e:
            return 0

    def process_sub(self, name, args=None):
        if args:
            for i, arg in enumerate(args):
                self.vars[f"VAR{i+1}"] = arg
        
        lines = self.subs[name]
        ptr = 0
        while ptr < len(lines):
            line = lines[ptr].strip()
            if line.startswith("(debug"):
                self.output.append(line)

            # 1. Variable Assignments (H121 = ...)
            elif '=' in line and not line.startswith('o'):
                parts = line.split('=')
                var_name = parts[0].strip().replace('#', 'VAR')
                self.vars[var_name] = self.evaluate_expression(parts[1])
            
            # 2. Logic: o804 if[...]
            elif 'if[' in line:
                condition = re.search(r'if\s*\[(.*?)\]', line).group(1)
                # Convert G-code EQ/GT to Python == / >
                cond_eval = condition.replace('EQ', '==').replace('GT', '>').replace('LT', '<')
                result = self.evaluate_expression(cond_eval)
                
                # Find matching endif/else
                depth = 1
                temp_ptr = ptr + 1
                else_ptr = -1
                while depth > 0:
                    if 'if[' in lines[temp_ptr]: depth += 1
                    if 'endif' in lines[temp_ptr]: depth -= 1
                    if depth == 1 and 'else' in lines[temp_ptr]: else_ptr = temp_ptr
                    temp_ptr += 1
                
                if result:
                    # Process inside if until else or endif
                    limit = else_ptr if else_ptr != -1 else temp_ptr - 1
                    self.interpret_block(lines[ptr+1 : limit])
                elif else_ptr != -1:
                    # Process else block
                    self.interpret_block(lines[else_ptr+1 : temp_ptr-1])
                
                ptr = temp_ptr - 1

            # 3. Subroutine Calls
            elif 'call' in line:
                match = re.search(r'o(\d+)\s+call\s*(?:\[\s*(.*?)\s*\])?', line)
                sub_num = match.group(1)
                sub_args = [self.evaluate_expression(a) for a in match.group(2).split(',')] if match.group(2) else []
                self.process_sub(sub_num, sub_args)

            # 4. Standard G-code Output
            elif line and not line.startswith('o') and not line.startswith('('):
                # Replace H variables in the string for final output
                out_line = line
                # Handle special EDM parameters like C010
                for var in sorted(self.vars.keys(), key=len, reverse=True):
                    out_line = out_line.replace(var, str(round(self.vars[var], 4)))
                self.output.append(out_line)

            ptr += 1

    def interpret_block(self, lines):
        # Helper to process blocks of lines (used by IF logic)
        temp_subs = {"temp": lines}
        orig_subs = self.subs
        self.subs = {"temp": lines, **orig_subs}
        self.process_sub("temp")
        self.subs = orig_subs

    def run(self, raw_text):
        # Pre-pass: Capture subroutines
        all_lines = raw_text.split('\n')
        current_sub = None
        main_code = []
        
        for line in all_lines:
            clean = line.strip()
            if 'sub' in clean and 'endsub' not in clean:
                current_sub = re.search(r'o(\d+)', clean).group(1)
                self.subs[current_sub] = []
            elif 'endsub' in clean:
                current_sub = None
            elif current_sub:
                self.subs[current_sub].append(clean)
            else:
                main_code.append(clean)
        
        # Execute Main Code
        self.subs['main'] = main_code
        self.process_sub('main')
        return "\n".join(self.output)

# YOUR G-CODE INPUT
gcode_input = """(      ON   OFF  MA   IP   SV  UP  DN  LN  LP   STEP  PL V  HP  PP C S  L  AL JS  JM OC   LD         )
C010 = 0150 0040 0000 0048 055 011 009 000 1111 0.000  + 21 000 11 0 22 02 10 075 03 0000 0002 
C004 = 0140 0040 0000 0053 060 012 006 000 1111 0.000  + 21 040 00 0 32 02 15 075 03 0000 0002 
C003 = 0070 0025 0000 0033 070 012 006 000 1111 0.000  + 21 040 00 0 42 02 15 075 03 0000 0002 
C002 = 0040 0025 0000 0023 085 012 006 000 1111 0.000  + 21 040 00 0 52 02 15 075 03 0000 0002 
C001 = 0010 0009 0000 0015 080 010 006 000 1111 0.000  + 22 040 00 0 63 03 18 050 00 0000 0002 

o800 sub
G00 XH661 YH662
G00 ZH663

H121 = H661-H771
H122 = H662-H772
H123 = H663-H773
H111 = SQRT[H121*H121+H122*H122+H123*H123]

MDIV8 AOD0.030 LEJL0.150 LEJS50
o804 if[0 EQ #1 or 1 EQ #1]
C010 H000=0.000 H222=0.187 H001=010 H002=1111
(debug, do_call_ui set_runmanual_table2_current_row_1)
o801 call 
o804 endif
(debug, do_call_ui unset_runmanual_table2_current_row_1)
o805 if[0 EQ #1 or 2 EQ #1]
C004 H000=0.034 H222=0.142 H001=010 H002=1111
(debug, do_call_ui set_runmanual_table2_current_row_2)
o801 call 
o805 endif
(debug, do_call_ui unset_runmanual_table2_current_row_2)
o806 if[0 EQ #1 or 3 EQ #1]
C003 H000=0.073 H222=0.099 H001=010 H002=1111
(debug, do_call_ui set_runmanual_table2_current_row_3)
o801 call 
o806 endif
(debug, do_call_ui unset_runmanual_table2_current_row_3)
o807 if[0 EQ #1 or 4 EQ #1]
C002 H000=0.101 H222=0.064 H001=010 H002=1111
(debug, do_call_ui set_runmanual_table2_current_row_4)
o801 call 
o807 endif
(debug, do_call_ui unset_runmanual_table2_current_row_4)
o808 if[0 EQ #1 or 5 EQ #1]
C001 H000=0.123 H222=0.044 H001=010 H002=1111
(debug, do_call_ui set_runmanual_table2_current_row_5)
o801 call 
o808 endif
(debug, do_call_ui unset_runmanual_table2_current_row_5)
o800 endsub

o801 sub
o810 if [H111 GT 0]
H666 = H771 + [H661 - H771] * H222 / H111
H777 = H772 + [H662 - H772] * H222 / H111
H888 = H773 + [H663 - H773] * H222 / H111
o810 else
o811 if [H881 GT 0]
H666 = H771 + H222
o811 else
H666 = H771
o811 endif

o811 if [H882 GT 0]
H777 = H772 + H222
o811 else
H777 = H772
o811 endif

o811 if [H883 GT 0]
H888 = H773 + H222
o811 else
H888 = H773
o811 endif 

o810 endif
STEPH000 LNH001 LPH002 G01 XH666 YH777 ZH888 M04
o801 endsub


G90 G11 G24 T84
H661   = 0.000
H771   = 0.000
H881   = 0
H662   = 0.000
H772   = 0.000
H882   = 0
H663   = 0.000
H773   = -1.900
H883   = 1
o800 call [0]
T85
do_beep 5000
do_light 2
(debug, do_call_ui reset_runmanual_table2_current_row)
M02
"""

flattener = GCodeFlattener()
print(flattener.run(gcode_input))