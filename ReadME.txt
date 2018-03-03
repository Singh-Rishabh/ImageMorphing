Sample Run : 
	g++ a2_rishabh_2016csb1054.cpp -o a `pkg-config --cflags --libs opencv`
	./a

I have done morping using delaunay triangulation in the images

User need to provide 2 images and 2 text files contaning tie points for the input images respectively

it will display the series of continious images using imshow.
By default it will display 20 morphs and if user want to change the number of morph then he can change the constant NUM_MORPH in the .cpp file.