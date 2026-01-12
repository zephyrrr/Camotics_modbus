from jinja2 import Template
import json
import os

# 1. Your UI data (this would come from your C++ app)
ui_data = {
    "feed_rate": 1200,
    "toolpaths": [
        {"x": 10.5, "y": 20.0},
        {"x": 15.2, "y": 25.5},
        {"x": 30.0, "y": 40.1}
    ]
}

template_content = '''C009  = 0090 0030 0000  0030 060 008 020 010 1111 0000  +  02 000  10  0  47  25  10 015  00 0000  2 ;
C008  = 0040 0020 0000  0020 060 006 020 010 1111 0000  +  02 000  10  0  47  25  25 112  00 0000  2 ;
C007  = 0015 0015 0000  0015 060 006 020 010 1111 0000  +  02 000  10  0  45  23  25 112  00 0000  2 ;
C006  = 0008 0008 0001  0008 090 006 030 010 1111 0000  +  02 000  11  0  45  23  25 112  00 0000  2 ;
C005  = 0005 0005 0001  0005 100 006 040 010 1111 0000  +  03 000  11  0  43  23  25 112  00 0000  2 ;
;
H100   = -000005.000;深度DEPTH=mm
H101   = +000000.150;单边STEP=μm
H102   = +000000.000;LN=(0自由;1HS摇动;2锁定;3思考;)&(0进给联动,回退摇动停止;1联动伺服;2回中心)&(0XY;1ZX;2YZ;3正交)
H104   = +001111.000; LP=4321(0无;1〇;2□;3🔶;4×) 
H105   = +000000.000;JM=(0普通抬刀;1十分之一)&(0缩0.02;1缩0.05;2缩中心6.5)



G11;
G90 G054 G09;
G00 X0.0 Y0.0 M98P0000;
T85;
M02;


N0000;
G90 G00 Z+1.0000;
T84;
;
MDIV16 AOD0.0300 LEJL0.150 LEJS60.0;
;
G12
/C010 STEPH101-0.130 LNH102 LPH104 JMH105 G01 ZH100+0.100 M04;
G11
/C009 STEPH101-0.090 LNH102 LPH104 JMH105 G01 ZH100+0.050 M04;
/C008 STEPH101-0.061 LNH102 LPH104 JMH105 G01 ZH100+0.030 M04;
/C007 STEPH101-0.041 LNH102 LPH104 JMH105 G01 ZH100+0.010 M04;
G00 Z+3.0000;
M99;'''

def generate_gcode(data, template_content):
    #if os.path.exists(template_content):
    #    # 2. Load the template file
    #    with open(template_content, 'r') as f:
    #        template_content = f.read()
    
    # 3. Create the Jinja2 template object
    template = Template(template_content)
    
    # 4. Render the data into the template
    rendered_gcode = template.render(data)
    
    return rendered_gcode

# Output the result
#print(window.getData("ui_data"))
final_gcode = generate_gcode(window.getData("ui_data"), template_content)
print(final_gcode)
if window:
    window.setData("result", final_gcode)
