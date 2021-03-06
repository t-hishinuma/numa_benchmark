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
thread_list = set(df["threads"].dropna())

figure_list = list()

for func in func_list:
    for thread in thread_list:
        # DOT
        if "dot" in func:
            data = df[ (df["func"] == func) & (df["threads"] == thread) ]
            arch_list = set(data["arch"].dropna())

            for arch in arch_list:
                data = df[ (df["func"] == func) & (df["threads"] == thread) & (df["arch"] == arch) ]
                size = data[["size"]].values.tolist()
                time = data[["time_sec"]].values.tolist()
                mem  = data[["mem_gb_s"]].values.tolist()
                perf = data[["perf_gflops"]].values.tolist()

                if arch == "cpu":
                    title = func + "_" + str(int(thread)) + "threads" + "_" + arch
                else:
                    title = func + "_" + arch

                plt.title(title)

                plt.xlabel("Vector size") 
                plt.xscale("log")

                plt.plot(size, time, label="Time [sec]", marker="o")
                plt.plot(size, perf, label="Performance [GFLOPS]", marker="o")   
                plt.plot(size, mem, label="Memory speed [GB/s]", marker="o")

                plt.legend(loc="best") 
                plt.savefig(title + ".png")
                plt.close()
                figure_list.append( title + ".png" )
        # GEMV
        if "gemv" in func:
            data = df[ (df["func"] == func) & (df["threads"] == thread) ]
            arch_list = set(data["arch"].dropna())
            for arch in arch_list:
                data = df[ (df["func"] == func) & (df["threads"] == thread) & (df["arch"] == arch) ]
                size = data[["size"]].values.tolist()
                time = data[["time_sec"]].values.tolist()
                perf = data[["perf_gflops"]].values.tolist()

                if arch == "cpu":
                    title = func + "_" + str(int(thread)) + "threads" + "_" + arch
                else:
                    title = func + "_" + arch

                plt.title(title)

                plt.xlabel("Matrix size N (N = M)") 
                #plt.xscale("log")

                plt.plot(size, time, label="Time [sec]", marker="o")
                plt.plot(size, perf, label="Performance [GFLOPS]", marker="o")   

                plt.legend(loc="best") 
                plt.savefig(title + ".png")
                plt.close()
                figure_list.append( title + ".png" )
        # GEMM
        if "gemm" in func:
            data = df[ (df["func"] == func) & (df["threads"] == thread) ]
            arch_list = set(data["arch"].dropna())
            for arch in arch_list:
                data = df[ (df["func"] == func) & (df["threads"] == thread) & (df["arch"] == arch) ]
                size = data[["size"]].values.tolist()
                time = data[["time_sec"]].values.tolist()
                perf = data[["perf_gflops"]].values.tolist()

                if arch == "cpu":
                    title = func + "_" + str(int(thread)) + "threads" + "_" + arch
                else:
                    title = func + "_" + arch

                plt.title(title)

                plt.xlabel("Matrix size N (N = M = K)") 
                #plt.xscale("log")

                plt.plot(size, time, label="Time [sec]", marker="o")
                plt.plot(size, perf, label="Performance [GFLOPS]", marker="o")   

                plt.legend(loc="best") 
                plt.savefig(title + ".png")
                plt.close()
                figure_list.append( title + ".png" )
        if 'fft' in func:
            data = df[ (df["func"] == func) & (df["threads"] == thread) ]
            arch_list = set(data["arch"].dropna())
            for arch in arch_list:
                data = df[ (df["func"] == func) & (df["threads"] == thread) & (df["arch"] == arch) ]
                size = data[["size"]].values.tolist()
                time = data[["time_sec"]].values.tolist()
                perf = data[["perf_gflops"]].values.tolist()

                if arch == "cpu":
                    title = func + "_" + str(int(thread)) + "threads" + "_" + arch
                else:
                    title = func + "_" + arch

                plt.title(title)

                plt.xlabel("size") 
                plt.xscale("log", basex=2)

                plt.plot(size, time, label="Time [sec]", marker="o")
                plt.plot(size, perf, label="Performance [GFLOPS]", marker="o")   

                plt.legend(loc="best") 
                plt.savefig(title + ".png")
                plt.close()
                figure_list.append( title + ".png" )

# create html

spec_data = df[ (df["type"] == "info")]
ph_cpu_num = int(spec_data[["physical_cpu"]].values)
cpu_model  = str(spec_data[["cpu"]].values)
cores      = int(spec_data[["cores"]].values)
memory     = str(spec_data[["memory"]].values)
GPU        = str(spec_data[["gpu"]].values)
GCC        = str(spec_data[["gcc"]].values)

html=open(output_file,'w')
print('<html>',file=html)
print('<body>',file=html)

print("<h3>", file=html)
print ("CPUname :", cpu_model, "<br>" , file=html)
print ("physical_CPU :", ph_cpu_num, "<br>" , file=html)
print ("cores :", cores, "<br>" , file=html)
print ("memory :", memory, "<br>" , file=html)
print ("GPU :", GPU, "<br>" , file=html)
print ("GCC :", GCC, "<br>" , file=html)
print("</h3>", file=html)

print("== Click to Open ==", file=html)

for func in func_list:
    print("<details>", file=html)
    s = "<summary>" + func + "</summary>"
    print(s, file=html)
    
    for thread in thread_list:
        data = df[ (df["func"] == func) & (df["threads"] == thread) ]
        arch_list = set(data["arch"].dropna())
        for arch in arch_list:
            if arch == "cpu":
                img = func + "_" + str(int(thread)) + "threads" + "_" + arch + ".png"
            else:
                img = func + "_" + arch + ".png"

            s = "<img src = \"" + img + "\">"
            print(s, file=html)
    print("</details>", file=html)

print('</body>',file=html)
print('</html>',file=html)
html.close()
