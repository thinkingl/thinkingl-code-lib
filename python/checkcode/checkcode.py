# 使用生成随机验证码图片
import os
import string
from random import randint, sample
from PIL import Image, ImageDraw, ImageFont, ImageFilter
# Image 负责处理图片
# ImageDraw 画笔
# ImageFont 文字
# ImageFileter 滤镜

def generateCheckCodeText() -> str:
    img_text = " ".join(sample(string.ascii_letters+string.digits, 5))
    return img_text

def generateCheckCodeImage( img_text ) -> Image:
    # 定义变量
    img_size = (150,50)        # 定义画布大小
    img_rgb = (255,255,255)    # 定义画布颜色，白色
    img = Image.new("RGB",img_size,img_rgb)

    # print(img_text.replace(' ',''))

    # 画图
    drow = ImageDraw.Draw(img)
    for i in range(10):
        # 随机画线
        drow.line([tuple(sample(range(img_size[0]),2)), tuple(sample(range(img_size[0]),2))], fill=(0,0,0))
    for i in range(99):
        # 随机画点
        drow.point(tuple(sample(range(img_size[0]),2)), fill=(0,0,0))

    # 文字
    font = ImageFont.truetype("simsun.ttc", 24)     # 定义文字字体和大小
    drow.text((6,6), img_text, font=font, fill="green")

    # 扭曲图片和滤镜
    params = [
        1 - float(randint(1,2)) / 100,
        0,
        0,
        0,
        1 - float(randint(1,10)) /100,
        float(randint(1,2)) / 500,
        0.001,
        float(randint(1,2)) / 500
    ]
    img = img.transform(img_size, Image.PERSPECTIVE, params)
    img = img.filter(ImageFilter.EDGE_ENHANCE_MORE)
    return img

# 展示图片
#img.show()

if __name__ == "__main__":
    dir = "e:/tmp/2021-08-30-验证码"
    imgDir = os.path.join( dir, 'img' )
    imgMapPath = os.path.join( dir, 'imgtable.txt' )
    if not os.path.isdir( imgDir ):
        os.makedirs( imgDir )
    imgMapFile = open(imgMapPath,'w',encoding='utf-8')
    for i in range(1000):
        text = generateCheckCodeText()
        img = generateCheckCodeImage( text )
        text = text.replace( ' ', '' )
        filename = str(i) + '.png'
        filepath = os.path.join( imgDir, filename )
        img.save( filepath )
        imgMapFile.write(str(i)+ ' ' + text + "\n" )
