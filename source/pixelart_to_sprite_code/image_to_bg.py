from numpy import asarray
from PIL import Image

img_name = input("image name: ")
file_name = input("file name destiny: ")
function_name = input("name for function: ")

img = Image.open(f"images/{img_name}.png")

rgb_img = img.convert('RGB')

len = img.size


sizex = len[0]//80
sizey = len[1]//60

px = img.load()
px_rgb = rgb_img.load()

cores = {}

with open(f"{file_name}.txt", "w") as arquivo:
    arquivo.write(f'int {function_name}() {{\n')

count = 0;
for i in range(0, 80):
    for j in range(0, 60):

        if(px[(j*sizey + sizey//2), (i*sizex + sizex//2)][3] == 0):
            r = 6
            g = 7
            b = 7

        else:
            r = int(round(((px_rgb[(j*sizey + sizey//2), (i*sizex + sizex//2)][0])*(7/254)),0))
            g = int(round(((px_rgb[(j*sizey + sizey//2), (i*sizex + sizex//2)][1])*(7/254)),0))
            b = int(round(((px_rgb[(j*sizey + sizey//2), (i*sizex + sizex//2)][2])*(7/254)),0))

        cor = "".join(f"cor{count}")
        rgb = "".join(f"{r} {g} {b}")

        if cores.get(rgb):
            instrucao = f"\tdraw_background_block({i}, {j}, {cores.get(rgb)});\n"
        else:
            cores[rgb] = cor
            instrucao = f"\tcolor_t cor{count} = {{{b}, {g}, {r}}};\n\tdraw_brackground_block({i}, {j}, cor{count});\n"
            count +=1

        with open(f"{file_name}.txt", "a") as arquivo:
            arquivo.write(instrucao)

        print(f"R: {r}\tG: {g}\t B:{b}")
    
with open(f"{file_name}.txt", "a") as arquivo:
    arquivo.write("\treturn 0;\n}\n")
print(img.getpalette)
print(rgb_img.getpalette)