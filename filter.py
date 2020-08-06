#!/bin/python3

def window_filter(input_data, window_size=10):
	output_data = [0 for _ in range(0, window_size)]

	for i in range(window_size, len(input_data)):
		output_data.append(
			sum(input_data[i - window_size : i]) / window_size
		)

	return output_data

def LPF(x, alpha=0.8):
	y = []
	first_run = True
	for i in x:
		if(first_run):
			first_run = False
			y.append( ( 1 - alpha ) * i )
		else:
			y.append( ( ( 1 - alpha ) * i ) + ( y[-1] * alpha ) )
	return y
