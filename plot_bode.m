% 读取数据
data = load('output.txt');

% 分离频率和增益数据
frequencies = data(:, 1);  % 第一列是频率
gains_dB = data(:, 2);     % 第二列是增益(dB)

% 绘制Bode图
figure;
semilogx(frequencies, gains_dB, 'b-', 'LineWidth', 2); % 在对数坐标轴上绘制
grid on;

% 添加标题和标签
title('Bode Diagram');
xlabel('Frequency (Hz)');
ylabel('Gain (dB)');

% 保存图像
print -dpng 'bode_plot.png';  % 将图像保存为PNG格式
