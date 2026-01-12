(Header: Starting Job)
G21 (Metric)
G90 (Absolute positioning)

{% for row in toolpaths %}
(Processing Row {{ loop.index }})
G1 X{{ row.x }} Y{{ row.y }} F{{ feed_rate }}
{% endfor %}

M30 (End of program)