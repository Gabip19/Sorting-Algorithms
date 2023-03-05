#include <math.h>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <cstdlib>

using namespace std;


#define PARENT(i) ((i - 1) / 2)
#define LEFT(i)   (2 * i + 1)
#define RIGHT(i)  (2 * i + 2)


void bubble_sort(int vec[], int len)
{
    int temp;
    for(int i=0; i<len; i++)
    {
        for(int j=len-1; j>i; j--)
        {
            if(vec[j]<vec[j-1])
            {
                temp=vec[j-1];
                vec[j-1]=vec[j];
                vec[j]=temp;
            }
        }
    }
}


//Algoritmul functioneaza foarte bine pe liste care sunt fie mini, fie deja aproape sortate
void insertion_sort(int vec[], int len)
{
    for(int j=1;j<len;j++)
    {
        int key=vec[j];
        int i = j-1;
        while(i>-1 and vec[i]>key)
        {
            vec[i+1]=vec[i];
            i=i-1;
        }
        vec[i+1]=key;
    }
}


void merge(int vec[], int p, int q, int r)
{
    //n1 si n2 sunt lungimile listelor presortate vec[p..q] si vec[q+1..r]
    int n1=q-p+1;
    int n2=r-q;
    //copiere aceste liste presortate in L si R
    int L[n1+1];
    int R[n2+1];
    for(int i=0;i<n1; i++)
    {
        L[i]=vec[p+i];
    }
    for(int j=0;j<n2; j++)
    {
        R[j]=vec[q+1+j];
    }


    //crearea unei valori pentru L si R care este mai mare decat
    //cea mai mare valoare din lista
    int largest;
    if(L[n1-1]<R[n2-1])
        largest=R[n2-1];
    else
        largest=L[n1-1];
    L[n1]=largest+1;
    R[n2]=largest+1;

    //concatenare liste L si R
    int i=0;
    int j=0;
    for(int k=p; k<=r; k++)
    {
        if (L[i]<=R[j])
        {
            vec[k]=L[i];
            i++;
        }
        else
        {
            vec[k]=R[j];
            j++;
        }
    }
}

void merge_sort_aux(int vec[], int p, int r)
{
    if(p<r)
    {
        int q=floor((p+r)/2);
        merge_sort_aux(vec,p,q);
        merge_sort_aux(vec,q+1,r);
        merge(vec,p,q,r);
    }
}

void merge_sort(int vec[], int len)
{
    merge_sort_aux(vec, 0, len - 1);
}


//in max-heap inainte de sortare. In max-heap  fiecare element este mai mare decat
//copilul sau, STANGA si DREAPTA.
class heap
{
    public:
        int *nodes;
        int length;
        int heap_size;
};


void max_heapify(heap vec, int index)
{
    int left,right,largest,exchange_temp;

    left = LEFT(index);
    right = RIGHT(index);

    if(left < vec.heap_size && vec.nodes[left] > vec.nodes[index])
    {
        largest = left;
    }
    else
    {
        largest = index;
    }

    if(right < vec.heap_size && vec.nodes[right] > vec.nodes[largest])
    {
        largest = right;
    }

    if(largest != index)
    {
        exchange_temp = vec.nodes[index];
        vec.nodes[index] = vec.nodes[largest];
        vec.nodes[largest] = exchange_temp;
        max_heapify(vec, largest);
    }
}


void build_max_heap(heap vec)
{
    vec.heap_size = vec.length;
    for(int i = vec.length/2; i>=0; i--)
    {
        max_heapify(vec, i);
    }
}

void heap_sort(int vec[], int len)
{
    int exchange_temp;
    heap tempheap;
    tempheap.length = len;
    tempheap.nodes = vec;
    tempheap.heap_size = len;
    build_max_heap(tempheap);

    for(int i= tempheap.length - 1; i>=1; i--)
    {
        exchange_temp = tempheap.nodes[0];
        tempheap.nodes[0] = tempheap.nodes[i];
        tempheap.nodes[i] = exchange_temp;
        tempheap.heap_size = tempheap.heap_size - 1;

        max_heapify(tempheap,0);
    }
}

//Algoritmul functioneaza excelent pe o lista random dar pe o lista aproape sortata
//algoritmul este destul de incet.
int partition(int vec[], int p, int r)
{
    int pivot, index, exchange_temp;
    pivot = vec[r];
    index = p - 1;
    for(int i = p; i < r; i++)
    {
        if(vec[i] <= pivot)
        {
            index++;
            exchange_temp = vec[i];
            vec[i] = vec[index];
            vec[index] = exchange_temp;
        }
    }
    exchange_temp = vec[r];
    vec[r] = vec[index+1];
    vec[index+1] = exchange_temp;
    return index+1;
}

void quicksort_aux(int vec[], int p, int r)
{
    int q;
    if(p<r)
    {
        q = partition(vec, p, r);
        quicksort_aux(vec, p, q-1);
        quicksort_aux(vec, q+1, r);
    }
}

void quick_sort(int vec[], int len)
{
    quicksort_aux(vec, 0, len-1);
}

void compare_algorithms() {
    
    using std::chrono::duration_cast;
    using std::chrono::nanoseconds;

    std::chrono::high_resolution_clock::time_point t1,t2;
    srand(time(0));

    int npointsmax = 100000, nave = 100, npoints = 46;
    double  bubble_timelist[npoints], insertion_timelist[npoints],merge_timelist[npoints], quick_timelist[npoints], heap_timelist[npoints];

    int *rlist1= new int[npointsmax];
    int *rlist2= new int[npointsmax];
    int *rlist3= new int[npointsmax];
    int *rlist4= new int[npointsmax];
    int *rlist5= new int[npointsmax];

    double nplist[npoints];
    nplist[0] = 1;
    for(int n=0;n<5;n++)
    {
        for(int j=2;j<11;j++)
        {
            nplist[9*n + j - 1] = j * pow(10,n);
        }
    }

    int icounter = 0;

    cout<<"Numarul de elemente random ce vor fi sortate:\n";

    for (int npointsi : nplist)
    {
        double bbtime,instime,hptime,mgtime,qktime;
        double bb_temp_timer = 0.0;
        double ins_temp_timer = 0.0;
        double hp_temp_timer = 0.0;
        double mg_temp_timer = 0.0;
        double qk_temp_timer = 0.0;
        cout<<"Va rugam sa asteptati. Sortam un array de "<<npointsi<<" elemente cu fiecare algoritm."<<endl;
        for(int j = 0; j< nave; j++)
        {
            //generare 5 copii ale aceleiasi liste (cate una pentru fiecare algoritm)
            for(int ii=0; ii<npointsi; ii++)
            {
                rlist1[ii]=rlist2[ii]=rlist3[ii]=rlist4[ii]=rlist5[ii]=rand() % 1000;
            }

            t1 = std::chrono::high_resolution_clock::now();
            merge_sort(rlist1,npointsi);
            t2 = std::chrono::high_resolution_clock::now();
            mgtime = std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count();
            mg_temp_timer += mgtime ;

            t1 = std::chrono::high_resolution_clock::now();
            heap_sort(rlist2,npointsi);
            t2 = std::chrono::high_resolution_clock::now();
            hptime = std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count();
            hp_temp_timer += hptime ;

            t1 = std::chrono::high_resolution_clock::now();
            quick_sort(rlist3,npointsi);
            t2 = std::chrono::high_resolution_clock::now();
            qktime = std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count();
            qk_temp_timer += qktime ;

            //bubble si insertion au complexitate O(n^2) in medie
            if(npointsi<=500)
            {
                t1 = std::chrono::high_resolution_clock::now();
                bubble_sort(rlist4,npointsi);
                t2 = std::chrono::high_resolution_clock::now();
                bbtime = std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count();
                bb_temp_timer += bbtime ;

                t1 = std::chrono::high_resolution_clock::now();
                insertion_sort(rlist5,npointsi);
                t2 = std::chrono::high_resolution_clock::now();
                instime = std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count();
                ins_temp_timer += instime   ;
            }
            else
            {
                bb_temp_timer = 0.0;
                ins_temp_timer = 0.0;
            }
        }
        merge_timelist[icounter] = mg_temp_timer/nave;
        heap_timelist[icounter] = hp_temp_timer/nave;
        quick_timelist[icounter] = qk_temp_timer/nave;
        insertion_timelist[icounter] = ins_temp_timer/nave;
        bubble_timelist[icounter] = bb_temp_timer/nave;
        icounter++;
    }
    FILE * resultsfile;
    resultsfile=fopen("rezultate_algoritmi.txt","w");
    fprintf(resultsfile, "N                BubbleSort   InsertionSort       MergeSort        HeapSort       QuickSort\n");
    for(int j=0;j< npoints;j++) fprintf(resultsfile, "%5e \t %10.2f \t %10.2f \t %10.2f \t %10.2f \t %10.2f \n",nplist[j], bubble_timelist[j], insertion_timelist[j], merge_timelist[j], heap_timelist[j], quick_timelist[j]);
    fclose(resultsfile);
    cout << "Apasati orice tasta" << endl;
}

void clearScreen() {
    if (system("CLS")) system("clear");
}

void readFile(string fileName)
{
    SetConsoleOutputCP( CP_UTF8 );
    setvbuf( stdout, nullptr, _IONBF, 0 );
    string STRING;
	std::ifstream infile;
	infile.open(fileName, std::ifstream::in);
    while(getline(infile,STRING))
    {
        cout << STRING << endl;
    }
	infile.close();
}

int main()
{
    int choice;
    bool appOn = true;
    while (appOn != false){
        cout << "*******************************" << endl;
        cout << "1. Merge Sort" << endl;
        cout << "2. Heap Sort" << endl;
        cout << "3. Quick Sort" << endl;
        cout << "4. Bubble Sort" << endl;
        cout << "5. Insertion Sort" << endl;
        cout << "6. Rulare algoritmi" << endl;
        cout << "7. Verificare rezultate" << endl;
        cout << "8. Iesire aplicatie" << endl;
        cout << "*******************************" << endl;
        cout << " Introduceti optiunea de la tastatura si apasati enter: ";
        cin >> choice;

        clearScreen();
        switch(choice) {
            case 1:
                readFile("teorie/merge.txt");
                break;
            case 2:
                readFile("teorie/heap.txt");
                break;
            case 3:
                readFile("teorie/quick.txt");
                break;
            case 4:
                readFile("teorie/bubble.txt");
                break;
            case 5:
                readFile("teorie/insertion.txt");
                break;
            case 6:
                compare_algorithms();
                break;
            case 7:
                readFile("rezultate_algoritmi.txt");
                break;
            case 8:
                exit(0);
                break;
            default:
                cout << "Nu ati introdus o optiune valida. \n";
                cout << "Incercati din nou.\n";
                break;
        }
        getch();
        clearScreen();
    }
    return 0;
}
