import yaml
import sys
import matplotlib
import pandas

matplotlib.use('Agg')
import matplotlib.pyplot as plt

input_file = sys.argv[1]
output_file = sys.argv[2]

yml = yaml.safe_load( open(input_file))
df = pandas.DataFrame(yml)

# func list
func_list = set(df["func"].dropna())

# threads list
thread_list = set(df["# of threads"].dropna())

figure_list = list()

for func in func_list:
    for thread in thread_list:
        # DOT
        if func == "ddot" or func == "sdot":
            data = df[ (df["func"] == func) & (df["# of threads"] == thread) ]
            arch_list = set(data["arch"].dropna())

            for arch in arch_list:
                data = df[ (df["func"] == func) & (df["# of threads"] == thread) & (df["arch"] == arch) ]
                size = data[["size"]].values.tolist()
                time = data[["time [s]"]].values.tolist()
                mem  = data[["mem [GB/s]"]].values.tolist()
                perf = data[["perf [GFLOPS]"]].values.tolist()

                if arch == "cpu":
                    title = func + "_" + str(int(thread)) + "threads" + "_" + arch
                else:
                    title = func + "_" + arch

                plt.title(title)

                plt.xlabel("Vector size") 
                plt.xscale("log")

                plt.plot(size, time, label="time [sec]")   
                plt.plot(size, perf, label="performance [GFLOPS]")   
                plt.plot(size, mem, label="memory speed [GB/s]")   

                plt.legend(loc="best") 
                plt.savefig(title + ".png")
                plt.close()
                figure_list.append( title + ".png" )
        # GEMM
        if func == "dgemm" or func == "sgemm":
            data = df[ (df["func"] == func) & (df["# of threads"] == thread) ]
            arch_list = set(data["arch"].dropna())
            for arch in arch_list:
                data = df[ (df["func"] == func) & (df["# of threads"] == thread) & (df["arch"] == arch) ]
                size = data[["size"]].values.tolist()
                time = data[["time [s]"]].values.tolist()
                perf = data[["perf [GFLOPS]"]].values.tolist()

                if arch == "cpu":
                    title = func + "_" + str(int(thread)) + "threads" + "_" + arch
                else:
                    title = func + "_" + arch

                plt.title(title)

                plt.xlabel("Matrix size N (N = M = K)") 
                #plt.xscale("log")

                plt.plot(size, time, label="time [sec]")   
                plt.plot(size, perf, label="performance [GFLOPS]")   

                plt.legend(loc="best") 
                plt.savefig(title + ".png")
                plt.close()
                figure_list.append( title + ".png" )

# create html

spec_data = df[ (df["type"] == "info")]
ph_cpu_num = int(spec_data[["# of physical cpu"]].values)
cpu_model  = str(spec_data[["CPU model"]].values)
cores      = int(spec_data[["# of cores"]].values)
memory     = str(spec_data[["# of memory"]].values)
GPU        = str(spec_data[["GPU"]].values)

html=open(output_file,'w')
print('<html>',file=html)
print('<body>',file=html)

print("<h3>", file=html)
print ("CPU Model name :", cpu_model, "<br>" , file=html)
print ("# of physical CPU :", ph_cpu_num, "<br>" , file=html)
print ("# of cores :", cores, "<br>" , file=html)
print ("# of memory :", memory, "<br>" , file=html)
print ("GPU :", GPU, "<br>" , file=html)
print("</h3>", file=html)


for fig in figure_list:
    s = "<img src = \"" + fig + "\">"
    print(s, file=html)

print('</body>',file=html)
print('</html>',file=html)
html.close()
