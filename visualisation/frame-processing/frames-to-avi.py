from subprocess import call
import os

def main():
	path = './output/frames/frame{0}.png'

	out_name = 'full_network_anim'

	call(['ffmpeg',
		'-f', 'image2',
		'-framerate', '25',
		'-i', path.format('%d'),
		'-b:v', '64M',
		'{0}.avi'.format(out_name)])

if __name__ == '__main__':
	main()
