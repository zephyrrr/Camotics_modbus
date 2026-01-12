from jinja2 import Template
import json

# 1. Your UI data (this would come from your C++ app)
ui_data = {
    "feed_rate": 1200,
    "toolpaths": [
        {"x": 10.5, "y": 20.0},
        {"x": 15.2, "y": 25.5},
        {"x": 30.0, "y": 40.1}
    ]
}

def generate_gcode(data, template_path):
    # 2. Load the template file
    with open(template_path, 'r') as f:
        template_content = f.read()
    
    # 3. Create the Jinja2 template object
    template = Template(template_content)
    
    # 4. Render the data into the template
    rendered_gcode = template.render(data)
    
    return rendered_gcode

# Output the result
final_gcode = generate_gcode(ui_data, 'template/dgjg1_template.gcode')
print(final_gcode)