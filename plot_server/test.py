import dash
import dash_core_components as dcc
import dash_html_components as html
import plotly.graph_objs as go  
import sys
import yaml
import numpy as np
import pandas

# data input
input_file = sys.argv[1]

yml = yaml.safe_load( open(input_file))
df = pandas.DataFrame(yml)

func_list = set(df["func"].dropna())
thread_list = set(df["threads"].dropna())

app = dash.Dash()


app.layout = html.Div(children=[
    html.Div(
        html.H1('goma graph',
            style = {'textAlign': 'center'})
        ),
    dcc.Dropdown(
        id = 'FunctionList',
        options = [{'label': i, 'value': i} for i in func_list],
        value = 'func'
        ),
    dcc.Dropdown(
        id = 'ThreadList',
        options = [{'label': i, 'value': i} for i in thread_list],
        value = 'thread'
        ),
    dcc.Graph(
        id="gomaGraph",
        )
    ])

# ③コールバック作成
@app.callback(
        dash.dependencies.Output('gomaGraph', 'figure'),
        [dash.dependencies.Input('FunctionList', 'value'),
         dash.dependencies.Input('ThreadList', 'value')
        ]
        )
def update_graph(val, th):
    dff = df[ (df['func'] == val) & (df['threads'] == th)]

    return {
        'data': [
            go.Scatter(
                x = dff["size"].dropna(),
                y = dff["perf_gflops"].dropna(),
                mode = "lines+markers",
                name = "Performance [GFLOPS]",
            ),
            go.Scatter(
                x = dff["size"].dropna(),
                y = dff["mem_gb_s"].dropna(),
                mode = "lines+markers",
                name = "memory [GB/s]",
            )
        ]
    }

if __name__ == '__main__':
    app.run_server(debug=True, host='0.0.0.0', port=4545)
