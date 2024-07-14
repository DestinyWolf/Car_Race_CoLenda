from numpy import asarray
from PIL import Image


img = Image.open("images/trash_bag_pixel.png")

rgb_img = img.convert('RGB')

len = img.size

if len[0] == len[1]:
    print("A imagem é quadrada")
    
else:
    print("A imagem não é quadrada")
    exit(1)

sizex = len[0]//20
sizey = len[1]//20

px = img.load()
px_rgb = rgb_img.load()

cores = {}

with open("sprite.txt", "w") as arquivo:
    arquivo.write('color_t pixeis[400];\n')

for i in range(0, 20):
    for j in range(0, 20):

        if(px[(j*sizey + sizey//2), (i*sizex + sizex//2)][3] == 0):
            r = 6
            g = 7
            b = 7

        else:
            r = int(round(((px_rgb[(j*sizey + sizey//2), (i*sizex + sizex//2)][0])*(7/254)),0))
            g = int(round(((px_rgb[(j*sizey + sizey//2), (i*sizex + sizex//2)][1])*(7/254)),0))
            b = int(round(((px_rgb[(j*sizey + sizey//2), (i*sizex + sizex//2)][2])*(7/254)),0))

        cor = "".join(f"cor{(i*20)+j}")
        rgb = "".join(f"{r} {g} {b}")

        if cores.get(rgb):
            instrucao = f"pixeis[{(i*20) + j}] = {cores.get(rgb)};\n\n"
        else:
            cores[rgb] = cor
            instrucao = f"color_t cor{(i*20)+j};\n\ncor{(i*20)+j}.blue = {b};\ncor{(i*20)+j}.green = {g};\ncor{(i*20)+j}.red = {r};\npixeis[{(i*20)+j}] = cor{(i*20)+j};\n\n"

        with open("sprite.txt", "a") as arquivo:
            arquivo.write(instrucao)

        print(f"R: {r}\tG: {g}\t B:{b}")
    
print(img.getpalette)
print(rgb_img.getpalette)