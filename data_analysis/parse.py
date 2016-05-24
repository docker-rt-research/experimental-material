#!/usr/bin/python
#
# Author: Philip Masek
#
# Usage: python parsefile.py -i [input file/folder]
# Description: Parses data derived from the project ODVpi
# Url: https://www.github.com/Pletron/ODVpi
#

import sys, getopt, os, numpy

def prRed(prt): print("\033[91m {}\033[00m" .format(prt))

def percentage(part, whole):
  return 1 * float(part)/float(whole)

def parse_data_file(inputfile, outputfile, deployment, kernel, load):
	piio = "camera" in inputfile.name or int(inputfile.name.split('/')[-1].split('-')[0]) > 40
	lines = inputfile.readlines()
	inputfile.close()

	# CLEAN OUT ERROR LINES
	for i,line in enumerate(lines):
		if "100;" in line:
			lines = lines[i:][:-1]
			break
	
	intercept = 10212021


	for i,line in enumerate(lines):
		index = int(line.split(';')[0])

		if index is 1 and i > 4:


			if piio:

				outputfile.write("%s;" % (deployment))
				outputfile.write("%s;" % (kernel))
				outputfile.write("%s;" % (load))
				
				for j in range(1,7):
					outputfile.write("%d;" % (int(lines[i-(j-1)].split(';')[1])-int(lines[i-j].split(';')[1])))
				
				outputfile.write("%d\n" % (int(lines[i].split(';')[1])	-int(lines[i-6].split(';')[1])))

			else:
				
				outputfile.write("%s;" % (deployment))
				outputfile.write("%s;" % (kernel))
				outputfile.write("%s;" % (load))

				for j in range(1,5):
					outputfile.write("%d;" % (int(lines[i-(j-1)].split(';')[1])-int(lines[i-j].split(';')[1])))

				outputfile.write("%d\n" % (int(lines[i].split(';')[1])	-int(lines[i-4].split(';')[1])))
				

	return 1



def main(argv):

	outputfolder = inputfile = ''
	camera = False
	try:
		opts, args = getopt.getopt(argv,"hi:o:c",["inputfile=", "outputfolder=", "-c [if piio data input]"])
	except getopt.GetoptError:
		print 'parsefile.py -i <input (file or folder)>'
		sys.exit(2)
	for opt, arg in opts:
		if opt == '-h':
			print 'parsefile.py -i <input file/folder> -o <output folder> -s <split data> -g'
			sys.exit()
		elif opt in ("-i", "--input"):
			inputfile = arg
		elif opt in ("-o", "--outputfolder"):
			outputfolder = arg.__add__('/')
		elif opt in ("-c", "--camera"):
			camera = True
	


	if inputfile is '':
		prRed("[Error::1] No input file or folder defined")
		sys.exit()
	else:
		print '\nParsing %s: \n' % ('file' if os.path.isfile(inputfile) else 'folder'), inputfile


	files = []
	if os.path.isdir(inputfile):
		for file in os.listdir(inputfile):
			files.append(os.path.join(inputfile, file))
	elif os.path.isfile(inputfile):
		files.append(inputfile)
	else:
		prRed("[Error::2] Input does not exist: %s" %inputfile)
		sys.exit()


	if not os.path.exists('./parsed'):
		os.makedirs('./parsed')
	if not os.path.exists('./parsed/%s' % outputfolder):
		os.makedirs('./parsed/%s' % outputfolder)


	files = [x for x in files if not x.split('/')[1].startswith('.')]


	deployment_contexts	= ['1','0']
	kernel_versions		= ['0','1']
	load_types			= ['0','1']

	outputfile = open('./parsed/%s%s-parsed.csv' % (outputfolder,files[0].split('/')[-2]), 'w+')
	

	if camera:
		outputfile.write("deployment;kernel;load;sleep;odv_oh2;pi_calc;diskio;camio;odv_oh1;total\n")
	else:
		outputfile.write("deployment;kernel;load;sleep;odv_oh2;pi_calc;odv_oh1;total\n")

	for filepath in files:
		try:
			scenario 	= (int(filepath.split('/')[-1].split('-')[0])-1) % 8 + 1
			
			if scenario%2 is 1:
				kernel 	= kernel_versions[0]
			else:
				kernel 	= kernel_versions[1]

			if scenario%4 is 3 or scenario%4 is 0:
				load 	= load_types[1]
			else:
				load 	= load_types[0]

			if scenario > 4:
				deployment = deployment_contexts[0]
			else:
				deployment = deployment_contexts[1]
			
			

			results = parse_data_file(open(filepath), outputfile, deployment, kernel, load)

			print "Parsed: %d\t%s\t%s\t%s" % (int(filepath.split('/')[-1].split('-')[0]), kernel, load, deployment)

		except Exception, e:
			prRed("Failed to parse %s" % filepath)
			print e
	
	outputfile.close()


if __name__ == '__main__':
	main(sys.argv[1:])

