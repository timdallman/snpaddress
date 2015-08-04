#include <iostream>
#include <string.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

////////////////////////////////CLASSES//////////////////////////////////

///////////////////////////////FUNCTIONS/////////////////////////////////

//convert string into int
double cstr_double(string& str_double)
{
    ostringstream osstr_double;
    osstr_double<<str_double<<endl;
    istringstream isstr_double(osstr_double.str());
    double doublestr_double;
    isstr_double>>doublestr_double;
    return (doublestr_double);
}


//read matric file into multimap
multimap <string, double> read_file(char *a)
{
	//global variables
	multimap <string,double>  mat;
	char split_char = '\t';
	string line;
	string name1;
	string name2;
	double dist;	

	ifstream infile (a);
	if (infile.is_open())
	{
		while ( getline (infile,line))
        	{
			istringstream split(line);
			string each;
			int count = 0;
			while(std::getline(split, each, split_char))
               		{
               			if(count == 0)
                 		{
					name1 = each;
					
                 		}
                    		if(count ==1)
                    		{
                        	 	name2 = each;

                    		}
				if(count ==2)
				{
	                    		dist = cstr_double(each);
				}
                  		count++;
			}
			//Makes multimap of name1:name2 = snp_dist
			string pair = name1+":"+name2;
			mat.insert(make_pair(pair,dist));
			
			string pair2 = name2+":"+name1;
			mat.insert(make_pair(pair2,dist));
			
			
		}
	}
	return mat;
}

vector <vector <string> > make_links(multimap <string,double> mat, string each)
{
	//global varaiables
	vector <vector <string> > links;
	vector <string> matches;
	string name1, name2;
	
	//set cutoff
	double co = cstr_double(each);
	multimap <string, double >::iterator  it;
	char split_char = ':';
	string old_name = "";
	//vector for mathches


	//Loop through matrix multimap
	for (it=mat.begin(); it!=mat.end(); it++)
	{
		
		//get pair		
		string pair = it->first;
		string splitter;
		istringstream split(pair);
		int count = 0;
		while(std::getline(split, splitter, split_char))
        	{
			if(count == 0)
			{
				name1 = splitter;
			}
			if(count == 1)
			{
				name2 = splitter;
			}
			count++;
		}		
		//if a new strain		
		if(name1 != old_name)
		{

			if(old_name != "")
			{
				links.push_back(matches);
			}
			//Create somewhere for the mathces
			matches.clear();
			matches.push_back(name1);
			old_name = name1;
		}
		double dist = it->second;
		// if the distance less or equal to threshold and the strain to the matches
		if (dist <= co)
		{	if(name1 != name2)
			{
				matches.push_back(name2);
			}
		}		

	}
	// add final comparison
	links.push_back(matches);

	
	return links;	
}


vector <vector <string> >  define_clusters(vector <vector <string> >  links)
{
		
	//initial dedup
	sort(links.begin(), links.end());    
	vector <vector <string> >::iterator d_it;
	d_it = unique(links.begin(), links.end());
	links.resize(distance(links.begin(),d_it));
	
	//globals

	vector <vector <string> >  clusters;
	clusters.resize(links.size());
	
	//first pass to reduce the links
	
	//iterate over first cluster	
	for (int i=0; i<links.size();i++)
	{ 		
		vector <string> strains = links[i];
		sort(strains.begin(), strains.end());
		
		//iterate over the rest
		for (int j=i+1; j<links.size();j++)
		{	
			
			vector <string> strains2 = links[j];
			//sort clusters
			sort(strains2.begin(), strains2.end());			
	
			//check they have anything in common
			//initalise iterator
			vector <string>::iterator int_it;
			//result vector
			vector <string> int_check;
			int_check.resize(strains.size()+strains2.size());
			int_it = set_intersection(strains.begin(), strains.end(), strains2.begin(), strains2.end(),int_check.begin());	
			//resize
			int_check.resize(int_it-int_check.begin());

			
			if (int_check.size() > 0)
			{
				//they have a common strain so find the union
			
				//initalise iterator
				vector <string>::iterator union_it;
				//results vector
				vector <string> cluster;
				cluster.resize(strains.size()+strains2.size());

				//find union	
				union_it = set_union(strains.begin(), strains.end(), strains2.begin(), strains2.end(),cluster.begin());	
			
				//resize	
				cluster.resize(union_it-cluster.begin());
				sort(cluster.begin(), cluster.end());
				links[j] = cluster;
		
			}			
							
		} 
		//return only unique vectors
		vector <vector <string> >::iterator d_it;
		d_it = unique(links.begin(), links.end());
		links.resize(distance(links.begin(),d_it));		
		
	}
	
	//second pass on reduced links make clusters	
	
	//iterate over first cluster	
	for (int i=0; i<links.size();i++)
	{ 
				
		vector <string> strains = links[i];
		sort(strains.begin(), strains.end());
		clusters[i]= strains;
		
		//iterate over the rest
		for (int j=i+1; j<links.size();j++)
		{	
			
			vector <string> strains2 = links[j];
			//sort clusters
			sort(strains2.begin(), strains2.end());			
			
			//check they have anything in common
			//initalise iterator
			vector <string>::iterator int_it;
			//result vector
			vector <string> int_check;
			int_check.resize(strains.size()+strains2.size());
			int_it = set_intersection(strains.begin(), strains.end(), strains2.begin(), strains2.end(),int_check.begin());	
			//resize
			int_check.resize(int_it-int_check.begin());

			if (int_check.size() > 0)
			{
				//they have a common strain so find the union
			
				//initalise iterator
				vector <string>::iterator union_it;
				//results vector
				vector <string> cluster;
				cluster.resize(strains.size()+strains2.size());

				//find union	
				union_it = set_union(strains.begin(), strains.end(), strains2.begin(), strains2.end(),cluster.begin());	
			
				//resize	
				cluster.resize(union_it-cluster.begin());
				sort(cluster.begin(), cluster.end());	
			
				//can we merge with other clusters
			
				
				for(int k=0; k<clusters.size();k++)
				{
					
					//get strains from clusters
					
					vector <string> c_strains = clusters[k];
					sort(c_strains.begin(), c_strains.end());
				
					//check they have anything in common
					//initalise iterator
					vector <string>::iterator int_it;
					//result vector
					vector <string> int_check;
					int_check.resize(c_strains.size()+cluster.size());

					
					int_it = set_intersection(c_strains.begin(), c_strains.end(), cluster.begin(), cluster.end(),int_check.begin());	
					//resize
					int_check.resize(int_it-int_check.begin());
					
					if (int_check.size() > 0)
					{					
				
							//initalise iterator
							vector <string>::iterator int_vector2;
							//results vector
							vector <string> cluster2;
							cluster2.resize(c_strains.size()+cluster.size());

							//find union	
							int_vector2 = set_union(c_strains.begin(), c_strains.end(), cluster.begin(), cluster.end(),cluster2.begin());	
							//resize	
							cluster2.resize(int_vector2-cluster2.begin());
							
							//update clusters
							clusters[i] = cluster2;
							clusters[k] = cluster2;

												
					}
				
				}
					
			}			
							
		} 
		
	}
	
		
	return clusters;
}



vector <vector <string> >  remove_duplicate_clusters(vector <vector <string> > clusters)
{
	sort(clusters.begin(), clusters.end());

	//return only unique vectors	
	vector <vector <string> >::iterator d_it;
	d_it = unique(clusters.begin(), clusters.end());
	clusters.resize(distance(clusters.begin(),d_it));
	
	return clusters;
}

void print_clusters(multimap <double, vector <vector <string> > > out_clusters, string co)
{
	//globals
	multimap <double, vector <vector <string> > >::reverse_iterator it;
	multimap <string, vector <int> > strain_list;
	
	//print threshold header
	cout << "#\t" << co << "\n"; 
	
	//loop through cutoffs
	for(it = out_clusters.rbegin(); it!=out_clusters.rend(); it++)
	{
		//create variables
		vector <vector <string> > clusters = it->second;
		double cutoff = it->first;
		//cout << cutoff;
		vector <vector <string> >::iterator c_it;
		//initialise cluster counter
		int i = 1;
		//loop through clusters
		for (c_it = clusters.begin(); c_it!=clusters.end(); c_it++)
		{
			//create variables
			vector <string>::iterator s_it;
			vector <string> strains = *c_it;			

			if (strains.size() > 0)
			{
				//loop through strains
				for (s_it = strains.begin(); s_it!=strains.end(); s_it++)
				{
					
					
					//add to multimap
					multimap <string, vector <int> >::iterator st_it;
					//find strain_list for strain
					st_it = strain_list.find(*s_it);
					if(st_it != strain_list.end())
					{
						//add cluster no
						st_it->second.push_back(i);
					}
					else
					{
						//create first instance
						vector<int> tmp;
						tmp.assign(1,i);
						strain_list.insert(make_pair(*s_it,tmp)); 
					}
					
				}
				//iterate cluster counter
				i++;
			}	
			
		}		
	}
	
	//print clusters
	multimap <string, vector <int> >::iterator p_it;
	for(p_it = strain_list.begin();p_it!=strain_list.end(); p_it++)
	{
		//create variables
		string name = p_it->first;
		vector <int> clust = p_it->second;
		cout << name << "\t";
		
		//iterator
		vector <int>::iterator v_it;
		string hier;
		for(v_it = clust.begin(); v_it!=clust.end();v_it++)
		{
		
			//convet to char and append to string
			std::stringstream ss;
			ss << *v_it;
			std::string str = ss.str();
			hier.append(str);
			hier.append(".");
		}
		//remove last .
		hier.pop_back();
		//print
		cout << hier;	
		cout << "\n";
	}
	
}



/////////////////////////////////Main////////////////////////////////////


int main(int argc, char *argv[])
{
	//IO
	if (argc <= 1)
	{
		cout << "Usage: " << argv[0] << "<snp matrix> <snp distances to use for clustering e.g 5:10:25:50>" << endl;
	        exit(1);
	}
	//globals
	char * matrix = argv[1];
	char * co = argv[2];
	char split_char = ':';
	multimap <string,double> mat;
	string each;
	istringstream split(co);
	
	multimap <double, vector <vector <string> > > out_clusters;

	//read matrix
	//cout << "reading matrix......\n";
	mat = read_file(matrix);
	// for each threshold
	while(std::getline(split, each, split_char))
        {
        	//make initial links
		vector <vector <string> > links = make_links(mat, each);
		//make single linkage clusters
		vector <vector <string> >  clusters = define_clusters(links);
		//remove duplicate clusters
		vector <vector <string> > dedup_clusters = remove_duplicate_clusters(clusters);
		//add to out_clusters multimap
		out_clusters.insert(make_pair(cstr_double(each),dedup_clusters));

	}	

	//print clusters
	print_clusters(out_clusters,co);
	
	return 0;
}
