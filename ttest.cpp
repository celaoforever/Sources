#include <stdio.h>
#include <string>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_statistics.h>
#include<cmath>
#include <iostream>
#define MAX_BUFFER_LENGTH 1024*10
using namespace std;

static const size_t npos = -1;  
vector<string> split(const string& src, string delimit, string null_subst=""); 

double* vector2array(vector<string> v)
{
        double* res=new double[v.size()];
        for(int i=0;i<v.size();i++)
        {
                res[i]=atof(v[i].c_str());
        };
        return res;
};

vector<string> split(const string& src, string delimit, string null_subst) 
{ 
    vector<string> temp;
    if( src.empty() || delimit.empty() )
            return temp;
    vector<string> v; 
    size_t deli_len = delimit.size(); 
    size_t index = npos, last_search_position = 0; 
    while( (index=src.find(delimit,    
        last_search_position))!=npos ) 
    { 
        if(index==last_search_position) 
            v.push_back(null_subst); 
        else 
            v.push_back( src.substr(last_search_position, index-  
            last_search_position) ); 
        last_search_position = index + deli_len; 
    } 
    string last_one = src.substr(last_search_position); 
    v.push_back( last_one.empty()? null_subst:last_one ); 
    return v; 
}   

double calPvalue(double* data,int size)
{
	double mean=gsl_stats_mean(data,1,size);
	double sv2=gsl_stats_variance(data,1,size);
	double T=mean/(sqrt(sv2)/sqrt(size-1));
	double p=gsl_cdf_tdist_Q(T,size-1);
	if(p==1)
		p=1-gsl_cdf_tdist_P(T,size-1);
	return p;
};

int main(int argc, char *argv[])
{
    char buf[ MAX_BUFFER_LENGTH ];
    int length = 0;
    if( (length = read( 0, buf,  MAX_BUFFER_LENGTH )) < 0 ) {
        return -1;
    }
    buf[length] = '\0';
    vector<string> line=split(string(buf),"\n");
    double* data=vector2array(line);
    double p=calPvalue(data,line.size());
    cout<<p<<endl;
    return 0;
}
