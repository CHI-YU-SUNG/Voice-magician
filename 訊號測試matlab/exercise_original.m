close all;
clear all;

%% readfile
[audio,fs] = audioread('noisefile.wav');
% change freq by yourself
freq = 300;
audio_denoise = lowpass(audio,freq,fs);

%% plot
figure
hold on;
x = linspace(-fs/2,fs/2,34048);
fftx1 = fft(audio);
fftsx1 = fftshift(fftx1);
ax1 = subplot(2,1,1);
plot(x,fftsx1); 
title(ax1, 'original signal');

ax2 = subplot(2,1,2);
fftx2 = fft(audio_denoise);
fftsx2 = fftshift(fftx2);
plot(x,fftsx2);
title(ax2,'after bandpass filter');
hold off;