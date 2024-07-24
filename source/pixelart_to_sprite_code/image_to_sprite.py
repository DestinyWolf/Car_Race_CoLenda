from numpy import asarray
from PIL import Image

img_name = input("image name: ")
file_name = input("file name destiny: ")

img = Image.open(f"images/{img_name}.png")

rgb_img = img.convert('RGB')

len = img.size


sizex = len[0]//20
sizey = len[1]//20

px = img.load()
px_rgb = rgb_img.load()

cores = {}

with open(f"{file_name}.txt", "w") as arquivo:
    arquivo.write('color_t pixeis[400];\n')

count = 0;
for i in range(0, 20):
    for j in range(0, 20):

        if(px[(i*sizey + sizey//2), (j*sizex + sizex//2)][3] == 0):
            r = 6
            g = 7
            b = 7

        else:
            r = int(round(((px_rgb[(i*sizey + sizey//2), (j*sizex + sizex//2)][0])*(7/254)),0))
            g = int(round(((px_rgb[(i*sizey + sizey//2), (j*sizex + sizex//2)][1])*(7/254)),0))
            b = int(round(((px_rgb[(i*sizey + sizey//2), (j*sizex + sizex//2)][2])*(7/254)),0))

        cor = "".join(f"cor{count}")
        rgb = "".join(f"{r} {g} {b}")

        if cores.get(rgb):
            instrucao = f"pixeis[{(i*20) + j}] = {cores.get(rgb)};\n"
        else:
            cores[rgb] = cor
            instrucao = f"color_t cor{count} = {{{r}, {g}, {b}}};\npixeis[{(i*20)+j}] = cor{count};\n"
            count +=1

        with open(f"{file_name}.txt", "a") as arquivo:
            arquivo.write(instrucao)

        print(f"R: {r}\tG: {g}\t B:{b}")
    
print(img.getpalette)
print(rgb_img.getpalette)