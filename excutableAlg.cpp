#include "excutableAlg.h"


#include "features.h"
#include "detectAlg.h"
#include "model.h"

#include "featureIO.h"
#include "cluster.h"


void Alg_SVM(vector<string>& trainFiles, vector<string>& testFiles, vector<int> labels)
{
	//train
	Mat feature;
	string writer  =  "./model/svm.model";
    featureExtrator(feature, trainFiles, writer); //train one-class SVM
    writeInSVM(writer, "./model/svm_train.model");
    trainSVMModel();
    
    //predict
    //printf("tag\n");
    int fileNumber  =  testFiles.size();
    if(labels.size() ! =  2*fileNumber)
    {
        printf("lable size error !\n");
        exit(1);
    }
    
    Mat feature;
    string writer  =  "./model/svm.model";
    for(int i  =  0; i < fileNumber; i++)
    {
        featureExtrator(feature, testFiles, writer);
        writeInSVM(writer, "./model/svm_test.model", labels);
        predictSVM(i);
    }
}

void Alg_LDA(vector<string>& file)
{
	int index  =  0;
	Mat vocab;
	
	Mat feature;
	string writer  =  "./model/lda.model";
	int kk  =  100;
    vector<string> vf;
    
	while(index < file.size())
	{
        if(!feature.empty())
			feature.release();
        if(!vf.empty())
            vf.clear();
        vf.push_back(file[index]);
        featureExtrator(feature, vf, writer);
		Mat indices;
		Mat centers;
		bool* flags;
        int* scope;
        cluster(feature, kk, indices, centers);

		int nl  =  centers.rows;
		int nc  =  centers.cols;
		flags  =  new bool[nl];
        scope  =  new int[nl];
		for(int i  =  0; i < nl; i ++)
		{	
            if(centers.row(i).isSubmatrix(vocab))
			{
				flags[i]  =  true;
				float* data  =  centers.ptr<float>(i);
                int j  =  0;
                for(; j < vocab.rows; j ++)
				{
                    int k  =  0;
                    for(; k < nc; k ++)
					{
                        if(fabs(vocab.at<float>(j, k) - data[k]) > 0.0001)
							continue;
					}
					if(k > =  nc)
						break;
				}
				scope[i]  =  j;
			}
			else
			{
				flags[i]  =  false;
				vocab.push_back(centers.row(i))	;
                scope[i]  =  vocab.rows - 1;
			}
		}
		
		FILE *file;
		file  =  fopen("./model/vocab.dat", "w");
		int* a  =  new int[nl];
		for(int i  =  0; i < nl; i ++)
		{
			a[i]  =  0;
            if(flags[i])
				continue;
			for(int j  =  0; j < nc; j ++)
                fprintf(file, "%f ", centers.at<float>(i, j));
            fprintf(file, "\n");
		}
		fclose(file);
		
		nl  =  feature.rows;
		for(int i  =  0; i < nl; i ++)
		{
			a[indices.row(i)(0)] ++;
		}
		nl  =  centers.rows;
		file  =  fopen("./model/feature.dat", "w");
		fprintf(file, "%d ", index);
		for(int i  =  0; i < nl; i ++)
		{
			fprintf(file, "%d:%d ", scope[i], a[i]);
		}
		fclose(file);
		
		index ++;
	}
	
	
	LDAEstimation();
}

void Alg_Sparse(vector<string>& file)
{
	Mat feature;
	string writer  =  "./model/X_train.mat";
	featureExtrator(feature, file, writer);
	writeInNormal(writer);
	
	trainDictionary();
	
	predictSparseCoeffient();
}

void Alg_GMM(vector<string>& file)
{
	Mat feature;
	string writer  =  "./model/gmm.model";
	featureExtrator(feature, file, writer);
	writeInNormal(writer);
	
	Mat indices;
	int K = 10;
	GMM(feature, indices, K);
}

void Alg_MeanShift(vector<string>& file)
{
	Mat feature;
	string writer  =  "./model/meanshift.model";
	featureExtrator(feature, file, writer);
	writeInNormal(writer);
	
	Mat indices;
	meanshiftCluster(feature, indices);
	int throw_type = 1;
	switch(throw_type)
	{
		case 1://
			
			break;
	}
}


