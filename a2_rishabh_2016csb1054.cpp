#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <string>
#define NUM_MORPH 20
using namespace cv;
using namespace std;

 
 // A structure to store triangle vertices
 struct triangle_ver
 {
     Point2f a,b,c;
     void print(){
        cout << a.x << " " << a.y << " " << b.x << " " << b.y << " "<< c.x << " " << c.y <<endl;  
     }
 };

// maximum function to calculate maximum of two numbers 
float maximum(float x, float y, float z) {
    float ma= max(x,y);; /* assume x is the largest */
    return max(ma,z);

}

int maximumm(int x, int y, int z, int w) {
    int ma= max(x,y);; /* assume x is the largest */
    int mb = max(ma,z);
    return max(mb,w);

}
// minimum function to calculate the minimum of the given points.
float minimum(float x, float y, float z) {
    float mi = min(x,y); /* assume x is the largest */
    return min(mi,z);
 
}
int minimumm(int x, int y, int z, int w) {
    int ma= min(x,y);; /* assume x is the largest */
    int mb = min(ma,z);
    return min(mb,w);

}

// function to calculate area


float area(float x1, float y1, float x2, float y2, float x3, float y3)
{
   return abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0);
}

// function to check if given point is inside the given triangle.
// Sourcecode taken from the Geeks for geeks. https://www.geeksforgeeks.org/check-whether-a-given-point-lies-inside-a-triangle-or-not/

bool isInside(float x1, float y1, float x2, float y2, float x3, float y3, float x, float y)
{
   float A = area (x1, y1, x2, y2, x3, y3);
   float A1 = area (x, y, x2, y2, x3, y3);
 
   float A2 = area (x1, y1, x, y, x3, y3);
 
   float A3 = area (x1, y1, x2, y2, x, y);
   
    return (abs(A-A1-A2-A3) <=2);
   //return (A == A1 + A2 +A3 );

}


 // Code to draw delaunay triangle Hint taken from 
 //https://mail.google.com/_/scs/mail-static/_/js/k=gmail.main.en.9CtlOcfiVYQ.O/m=pds,pdl,pdit,m_i,pdt,t/am=_p6AHZC2HwgA7MSwgyiNQJj9mUEuCTg5Axn__3sAIAA1ALgC-Jv7ADoHAAAAAAAAAAAAAAAAAAAAWCQ-AQ/rt=h/d=1/rs=AHGWq9BA4iCsSQasy5axOR9RRF7LzK6Q6w
static vector<triangle_ver> draw_delaunay( Mat& img, Subdiv2D& subdiv )
{
    vector<triangle_ver> vertices;
    vector<Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    vector<Point2f> pt(3);
    Size size = img.size();
    Rect rect(0,0, size.width, size.height);
 	cout << size.width << " "<<size.height <<endl;
    for( size_t i = 0; i < triangleList.size(); i++ )
    {
        Vec6f t = triangleList[i];

        pt[0] = Point2f((t[0]), (t[1]));
        pt[1] = Point2f((t[2]), (t[3]));
        pt[2] = Point2f((t[4]), (t[5]));
        // triangle_ver tri;
        //     tri.a=Point2f(pt[0].x,pt[0].y);
        //     tri.b=Point2f(pt[1].x,pt[1].y);
        //     tri.c=Point2f(pt[2].x,pt[2].y);
        //     vertices.push_back(tri);
        //cout << "P0 "<<pt[0].x << " " <<pt[0].y << " P1 "<<pt[1].x << " " <<pt[1].y << " P2 " << pt[2].x << " " <<pt[2].y << endl;
        if ( (int)rect.contains(pt[0]) && (int)rect.contains(pt[1]) && (int)rect.contains(pt[2]))
        {
            
            triangle_ver tri;
            tri.a=Point2f(pt[0].x,pt[0].y);
            tri.b=Point2f(pt[1].x,pt[1].y);
            tri.c=Point2f(pt[2].x,pt[2].y);
            vertices.push_back(tri);
        }
    }
    return vertices;
}


 

// Main Function to call every other function
int main( int argc, char** argv){
	cout << "Enter the names of two the images for morphing "<<endl;
	string a1,a2;
	cin >> a1 >>a2;
	Mat img = imread(a1.c_str());
    Mat img_out = imread(a2.c_str());
    // Keep a copy around
    Mat img_orig = img.clone();
    cout << "rows " << img_orig.rows << " cols "<<img_orig.cols<<endl;
    vector<Point2f> input_points;
    vector<Point2f> output_points;
    cout << "Enter the name of the file for tie points <Input Tie points> <Output Tie points>: ";
    string b1,b2;
    cin >> b1 >> b2;
    ifstream ifs(b1.c_str());
    ifstream ofs(b2.c_str());
    int x, y;
    int x1,y1;
    while(ifs >> x >> y && ofs >> x1 >> y1)
    {
        input_points.push_back(Point2f(x,y));
        output_points.push_back(Point2f(x1,y1));

    }
    int count_img = 0;
    for(float t=0;t<=1.001;t = t+(1.0/NUM_MORPH)){
        Mat intermediate_image(img_orig.rows*(1-t) + img_out.rows*t,img_orig.cols*(1-t)+img_out.cols*t,CV_8UC3, Scalar(0,0,0)); 
        vector<Point2f> intermediate_points;
        for(int i=0;i<input_points.size(); i++){
            float int_x = (input_points[i].x * (1-t)) + (output_points[i].x * t);
            float int_y = (input_points[i].y * (1-t)) + (output_points[i].y * t);
            intermediate_points.push_back(Point2f(int_x,int_y));
        }
        Size size = intermediate_image.size();
        cout << "size " << size.width << " " << size.height <<endl;
        Rect rect(0, 0, size.width, size.height);
     
        // Create an instance of Subdiv2D
        Subdiv2D subdiv(rect);

        for( vector<Point2f>::iterator it = intermediate_points.begin(); it != intermediate_points.end(); it++)
        {
            subdiv.insert(*it);
        }
        // Making Delaunay triangles
        vector<triangle_ver> intermediate_triangle = draw_delaunay( intermediate_image, subdiv );
        vector<triangle_ver> input_triangle, output_triangle;
        

        //MAKING INPUT TRIANGE and OUTPUT TRIANGLE
        

        for (int i=0; i<intermediate_triangle.size(); i++){
            Point2f point_tri = intermediate_triangle[i].a;
            int index1,index2,index3;
            index2 = index3 = index1 = -1;
            //cout << "Point " << point_tri <<endl;
            for(int j=0;j<intermediate_points.size(); j++){
                if (abs(point_tri.x - intermediate_points[j].x)< 0.001 && abs(point_tri.y - intermediate_points[j].y)<0.001 ){
                    index1 = j;break;
                }
            }

            point_tri = intermediate_triangle[i].b;
            for(int j=0;j<intermediate_points.size(); j++){
                if (abs(point_tri.x - intermediate_points[j].x)< 0.001 && abs(point_tri.y - intermediate_points[j].y)<0.001){
                    index2 = j;break;
                }
            }
            point_tri = intermediate_triangle[i].c;
            for(int j=0;j<intermediate_points.size(); j++){
                if (abs(point_tri.x - intermediate_points[j].x)< 0.001 && abs(point_tri.y - intermediate_points[j].y)<0.001){
                    index3 = j;
                    break;
                }
            }
            triangle_ver i_triangle,o_triangle;
            i_triangle.a = input_points[index1]; i_triangle.b = input_points[index2]; i_triangle.c = input_points[index3];
            o_triangle.a = output_points[index1]; o_triangle.b = output_points[index2]; o_triangle.c = output_points[index3];
            input_triangle.push_back(i_triangle);
            output_triangle.push_back(o_triangle);

        }

        // for (int i=0;i<intermediate_triangle.size(); i++){
        // 	intermediate_triangle[i].print();
        // }
        // cout <<"jjjjjjjjjjjjjjjj" <<endl;
        for(int i=0;i<intermediate_triangle.size();i++){
            float* constant_arr_input = new float[6];
            float* constant_arr_output = new float[6];
            Mat constant_arr_input1(2,3,CV_32FC3,Scalar(0,0,0));
            Mat constant_arr_output1(2,3,CV_32FC3,Scalar(0,0,0));
            Point2f azz[3];
            azz[0] = intermediate_triangle[i].a; azz[1] = intermediate_triangle[i].b; azz[2] = intermediate_triangle[i].c; 
            Point2f azo[3];
            azo[0] = input_triangle[i].a; azo[1] = input_triangle[i].b; azo[2] = input_triangle[i].c; 
            constant_arr_input1 = getAffineTransform(azz,azo);

            Point2f azi[3];
            azi[0] = output_triangle[i].a; azi[1] = output_triangle[i].b; azi[2] = output_triangle[i].c; 
            constant_arr_output1 = getAffineTransform(azz,azi);
            int ma=0;
            for(int j=0;j<2;j++){
                for(int k=0;k<3;k++){
                    constant_arr_input[ma] = constant_arr_input1.at<double>(j,k);
                    ma++;
                }

            }

            ma=0;
            for(int j=0;j<2;j++){
                for(int k=0;k<3;k++){
                    constant_arr_output[ma] = constant_arr_output1.at<double>(j,k);
                    ma++;
                }

            }

            float maxx = maximum(intermediate_triangle[i].a.x,intermediate_triangle[i].b.x,intermediate_triangle[i].c.x);
            float maxy = maximum(intermediate_triangle[i].a.y,intermediate_triangle[i].b.y,intermediate_triangle[i].c.y);
            float minx = minimum(intermediate_triangle[i].a.x,intermediate_triangle[i].b.x,intermediate_triangle[i].c.x);
            float miny = minimum(intermediate_triangle[i].a.y,intermediate_triangle[i].b.y,intermediate_triangle[i].c.y);

            for(float r = miny; r<=maxy + 1 ; r++){
                for(float c = minx; c<= maxx + 1; c++){
                    if(isInside(intermediate_triangle[i].a.x, intermediate_triangle[i].a.y, intermediate_triangle[i].b.x,intermediate_triangle[i].b.y,intermediate_triangle[i].c.x, intermediate_triangle[i].c.y,c,r)){
                        float tempxi = constant_arr_input[0]*c+ constant_arr_input[1]*r+ constant_arr_input[2];
                        float tempyi = constant_arr_input[3]*c+ constant_arr_input[4]*r + constant_arr_input[5];
                        // if (r == 100)
                        // cout << r << " " << c <<endl;
                        float tempxo = constant_arr_output[0]*c+ constant_arr_output[1]*r + constant_arr_output[2];
                        float tempyo = constant_arr_output[3]*c+ constant_arr_output[4]*r + constant_arr_output[5];

                        intermediate_image.at<Vec3b>(r,c) = t*img_out.at<Vec3b>(tempyo,tempxo) + (1-t)*img_orig.at<Vec3b>(tempyi,tempxi);     
                    }
                }
            }


        }
        int count = 0 ;
        int count1 = 0;
        for(int r=2;r<intermediate_image.rows-2;r++){
        	for (int c=2;c<intermediate_image.cols-2;c++){
        		if(intermediate_image.at<Vec3b>(r,c)[0] == 0 && intermediate_image.at<Vec3b>(r,c)[1] == 0 && intermediate_image.at<Vec3b>(r,c)[2] == 0){
        			intermediate_image.at<Vec3b>(r,c)[0] = (intermediate_image.at<Vec3b>(r-1,c)[0] + intermediate_image.at<Vec3b>(r+1,c)[0] + intermediate_image.at<Vec3b>(r,c-1)[0] + intermediate_image.at<Vec3b>(r,c+1)[0] + intermediate_image.at<Vec3b>(r-1,c-1)[0] + intermediate_image.at<Vec3b>(r+1,c-1)[0] + intermediate_image.at<Vec3b>(r+1,c-1)[0] + intermediate_image.at<Vec3b>(r+1,c+1)[0] )/8;
        			intermediate_image.at<Vec3b>(r,c)[1] = (intermediate_image.at<Vec3b>(r-1,c)[1] + intermediate_image.at<Vec3b>(r+1,c)[1] + intermediate_image.at<Vec3b>(r,c-1)[1] + intermediate_image.at<Vec3b>(r,c+1)[1] + intermediate_image.at<Vec3b>(r-1,c-1)[1] + intermediate_image.at<Vec3b>(r+1,c-1)[1] + intermediate_image.at<Vec3b>(r+1,c-1)[1] + intermediate_image.at<Vec3b>(r+1,c+1)[1] )/8;
        			intermediate_image.at<Vec3b>(r,c)[2] = (intermediate_image.at<Vec3b>(r-1,c)[2] + intermediate_image.at<Vec3b>(r+1,c)[2] + intermediate_image.at<Vec3b>(r,c-1)[2] + intermediate_image.at<Vec3b>(r,c+1)[2] + intermediate_image.at<Vec3b>(r-1,c-1)[2] + intermediate_image.at<Vec3b>(r+1,c-1)[2] + intermediate_image.at<Vec3b>(r+1,c-1)[2] + intermediate_image.at<Vec3b>(r+1,c+1)[2] )/8;
        		}
        	}
        }
        // string strf = boost::lexical_cast<string>(f_val); 

        imshow("morph images ",intermediate_image);
        
        waitKey(50);
    }
    waitKey();
     
    
 
    return 0;
}
