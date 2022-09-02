#include "rulermodel.h"
double cmyif(const double *x)
{
    if(fabs(x[0])>1e-7)
      return x[1];
    else return -1e+100;
}
vector<string> split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

double cmyelse(const double *x)
{
    return x[0];
}
RulerModel::RulerModel(int d)
{
    dimension = d;
    cprogram = new RulerCprogram(dimension);
    setStartSymbol(cprogram->getStartSymbol());
    mparser.resize(10);
    for(int i=0;i<(int)mparser.size();i++)
    {
        mparser[i].AddFunction("myif",cmyif,2);
        mparser[i].AddFunction("myelse",cmyelse,1);
    }
    vars="";
    for(int i=0;i<dimension;i++)
    {
        char str[100];
        sprintf(str,"x%d",i+1);

        vars=vars+str;
        if(i<dimension-1) vars=vars+",";
    }
}

double  RulerModel::eval(Data &x)
{
    if(lastExpr.size()==0) return 1e+10;
    double finalValue = -1e+100;
    double *xx= new double[dimension];
    for(int i=0;i<dimension;i++) xx[i]=x[i];
    for(int j=0;j<(int)clist.size();j++)
    {

        double vv = mparser[j].Eval(xx);
        if(vv>-1e+10)
        {
            finalValue = vv;
            break;
        }
    }
    delete[] xx;
    return finalValue;
}

static double rmax(double a,double b){return a>b?a:b;}

double  RulerModel::evalDeriv(Data &x,int pos)
{
    if(lastExpr.size()==0) return 1e+10;
    for(int i=0;i<dimension;i++)
    {
        double eps=pow(1e-18,1.0/3.0)*rmax(1.0,fabs(x[i]));
        x[i]+=eps;
        double v1=eval(x);
        x[i]-=2.0 *eps;
        double v2=eval(x);
        double g=(v1-v2)/(2.0 * eps);
        x[i]+=eps;
        if(i==pos) return g;
    }
    return -1;
}

double  RulerModel::evalSecondDeriv(Data &x,int pos)
{
    if(lastExpr.size()==0) return 1e+10;
    for(int i=0;i<dimension;i++)
    {
        double eps=pow(1e-18,1.0/3.0)*rmax(1.0,fabs(x[i]));
        x[i]+=eps;
        double v1=evalDeriv(x,pos);
        x[i]-=2.0 *eps;
        double v2=evalDeriv(x,pos);
        double g=(v1-v2)/(2.0 * eps);
        x[i]+=eps;
        if(i==pos) return g;
    }
    return -1;
}

void    RulerModel::setChromosome(vector<int> &g)
{
    int redo = 0;

    printf("g=[");
    for(int i=0;i<g.size();i++)
        printf("%d ",g[i]);
    printf("\n");
    string str = printRandomProgram(g,redo);
    if(redo>=REDO_MAX){ lastExpr="";return ;}
    clist = split(str,"#");

    //QString qstr=str.c_str();
    //QStringList subStr  = qstr.split("#");
    for(int i=0;i<clist.size();i++)
    {
        string st =clist[i];
        int d = mparser[i].Parse(st,vars);
        if(d!=-1)
        {
            lastExpr = "";
          //  printf("Wrong expression %s value = %d \n",st.c_str(),d);
            return ;
        }

    }

     lastExpr = str;
}

string  RulerModel::printF(vector<int> &genome)
{
    string ret = "";
    int redo = 0;

    string str = printRandomProgram(genome,redo);
    if(redo>=REDO_MAX) return ret;
    vector<string> mclist = split(str,"#");

    //QString qstr=str.c_str();
    //QStringList subStr  = qstr.split("#");

    for(int i=0;i<mclist.size();i++)
    {
        string st =mclist[i];
        int d = cprogram->Parse(st);
        if(!d)
        {
            //printf("Wrong expression %s \n",st.toStdString().c_str());
            return ret;
        }
        if(st.rfind("myif",0)==0)
        {
               string x = st.substr(5,st.size()-6);
               vector<string> ele = split(x,",");
               if(i>0)
                   ret=ret+"else\n";
               ret=ret+"if("+ele[0]+") value="+ele[1];
               ret+="\n";
        }
        else
        if(st.rfind("myelse",0)==0)
        {
            string x = st.substr(7,st.size()-8);
            ret = ret+"else value="+x;
        }
    }
    return ret;
}

RulerModel::~RulerModel()
{
    delete cprogram;
}
