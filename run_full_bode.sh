#!/bin/bash

# Step 1: 编译并运行生成矩阵的C程序
echo "正在编译并运行矩阵生成程序..."
gcc generate_matrix.c -o generate_matrix -lm
if [ $? -ne 0 ]; then
  echo "矩阵生成程序编译失败！"
  exit 1
fi

./generate_matrix
if [ $? -ne 0 ]; then
  echo "矩阵生成程序运行失败！"
  exit 1
fi
echo "矩阵生成完成，已生成 in.txt 文件。"

# Step 2: 编译并运行Bode计算的C程序
echo "正在编译并运行Bode计算程序..."
gcc bode.c -o bode -lm
if [ $? -ne 0 ]; then
  echo "Bode计算程序编译失败！"
  exit 1
fi

./bode
if [ $? -ne 0 ]; then
  echo "Bode计算程序运行失败！"
  exit 1
fi
echo "Bode计算完成，已生成 output.txt 文件。"

# Step 3: 运行Octave脚本生成Bode图
echo "正在运行Octave脚本生成Bode图..."
octave --silent --eval "plot_bode"
if [ $? -ne 0 ]; then
  echo "Octave脚本运行失败！"
  exit 1
fi
echo "Bode图生成成功，已保存为 bode_plot.png。"

# Step 4: 打开生成的Bode图
echo "正在打开生成的Bode图..."
open bode_plot.png &

echo "流程完成。"
