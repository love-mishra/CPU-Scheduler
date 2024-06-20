#include <bits/stdc++.h>
 
using namespace std;
int wt[100], bt[100], at[100], tat[100], n, p[100];
float awt[5], atat[5], idle[5];
vector<int> start,finish;
vector<string> state;
// void create_file_csv(char * filename, int a[][2], int n, int m) {
//   printf("\n Creating %s.csv file", filename);
//   FILE * fp;
//   int i, j;
//   filename = strcat(filename, ".csv");
//   fp = fopen(filename, "w+");
//   fprintf(fp, "FCFS, SJF, RR, SRTF, Innovative Algo");
//   for (i = 0; i < m; i++) {
//     fprintf(fp, "\n%d", i + 1);
//     for (j = 0; j < n; j++)
//       fprintf(fp, ",%d ", a[i][j]);
//   }
//   fclose(fp);
//   printf("\n %sfile created", filename);
// }

void input() {

    int c;
    cout<<"Enter 0 if you want to enter the processes as offline queries and enter 1 if you want to enter the processes as online queries\t";
    cin>>c;

    if(c == 0){
        cout<<"Enter Number of processes: ";
        cin>>n;
        int i;

        vector<pair<pair<int,int>,int> > v(n);

        for (i = 0; i < n; i++) p[i] = i + 1;

        for (i = 0; i < n; i++) {
            cout<<"Enter Burst Time of process "<<i+1<<": ";
            cin>>bt[i];
            v[i].first.second=bt[i];
            cout<<"Enter Arrival Time of process "<<i+1<<": ";
            cin>>at[i];
            v[i].first.first=at[i];
            v[i].second=i+1;
        }
    }

    else{
        int time = 10;
        cout<<"Enter the time until which processes arrive\t:";
        cin>>time;

        for(int i = 0; i <= time; i++){
            int num;
            cout<<"Enter the number of processes arriving at time "<<i<<":\t";
            cin>>num;
            n += num;

            for(int x = 1; x <= num; x++){
                int pid;
                cout<<"Enter the process id for process arriving:\t";
                cin>>pid;
                cout<<"Enter the burst time for process id "<<pid<<":\t";
                cin>>bt[pid-1];
                at[pid-1] = i;
            }
        }
    }

    vector<pair<pair<int,int>,int> > v(n);
    for(int i = 0; i < n; i++){
        v[i].first.first = at[i];
        v[i].first.second = bt[i];
        v[i].second = i+1;
    }

    int i;
    sort(v.begin(),v.end());
    for (i = 0; i < n; i++) p[i] = v[i].second;
    for (i = 0; i < n; i++) {
        bt[i]=v[i].first.second;
        at[i]=v[i].first.first;
    }
    for (i = 0; i < 5; i++) {
        awt[i] = 0.0;
        atat[i] = 0.0;
    }
}


void fcfs() {
    start.clear();
    finish.clear();
    state.clear();
    int time = at[0] + bt[0];
    idle[0]=at[0];
    if(at[0]){
        start.push_back(0);
        finish.push_back(at[0]);
        state.push_back("Idle");
    }
    start.push_back(at[0]);
    finish.push_back(time);
    state.push_back("Id["+to_string(p[0])+"]");
    wt[0] = 0;
    atat[0] = tat[0] = bt[0];
    for (int i = 1; i < n; i++) {
        if(at[i]<=time){
            start.push_back(time);
            wt[i] = time - at[i];
            time += bt[i];
            finish.push_back(time);
            state.push_back("Id["+to_string(p[i])+"]");
            awt[0] += wt[i];
            tat[i] = wt[i] + bt[i];
            atat[0] += tat[i];
        }
        else{
            start.push_back(time);
            finish.push_back(at[i]);
            state.push_back("Idle");
            start.push_back(at[i]);
            idle[0] += at[i] - time;
            time = bt[i] + at[i];
            finish.push_back(time);
            state.push_back("Id["+to_string(p[i])+"]");
            wt[i] = 0;
            awt[0] += wt[i];
            tat[i] = wt[i] + bt[i];
            atat[0] += tat[i];
        }
    }
  atat[0] /= n;
  awt[0] /= n;
  printf("SR.\tA.T.\tC.T.\tB.T.\tW.T.\tT.A.T.\n");
  for (int i = 0; i < n; i++) {
    printf("%3d\t%3d\t%3d\t%3d\t%3d\t%4d\n", p[i], at[i],at[i]+tat[i], bt[i], wt[i], tat[i]);
  }
  cout<<endl;
  cout<<"Starting Time \t";
  for (int i = 0; i < start.size(); i++) {
    cout<<start[i]<<"\t";
  }
  cout<<endl;
  cout<<"Finishing Time \t";
  for (int i = 0; i < start.size(); i++) {
    cout<<finish[i]<<"\t";
  }
  cout<<endl;
  cout<<"State\t\t";
  for (int i = 0; i < start.size(); i++) {
    cout<<state[i]<<"\t";
  }
  cout<<endl;
  cout<<endl;
}

int median(int bt[]){
    vector<int>v;
    for(int i=0;i<n;i++) if(bt[i]) v.push_back(bt[i]);
    sort(v.begin(),v.end());
    if(v.size()%2) return v[v.size()/2];
    return (v[v.size()/2]+v[v.size()/2-1])/2;
}

void innovative() {
    start.clear();
    finish.clear();
    state.clear();
    int count = 0, time=0, smallest = 0,wait_time=0,turnaround_time=0;
    vector<int> x(n);
    double avg = 0, tt = 0, end;
    for (int i = 0; i < n; i++) {
      x[i] = bt[i];
    }
    printf("\nProcess ID\t\tArrivalTime\tBurst Time\t Turnaround Time\t Waiting Time\n");
    for (time = 0; count != n;){
        int mnn = 1e9,counter=0;
        int y = 0 , mn = 1e9;
        for (int i = 0; i < n; i++) {
            if (at[i] <= time && bt[i] > 0) y = 1;
        }
        if(y == 0){
            for (int i = 0; i < n; i++) {
                if(bt[i] > 0)
                mn = min(mn , at[i]);
            }
            start.push_back(time);
            state.push_back("Idle");
            idle[4] += mn - time;
            time = mn;
            finish.push_back(time);
        }
        for (int i = 0; i < n; i++) {
            if (at[i] <= time && bt[i] < mnn && bt[i] > 0) {mnn=bt[i];smallest = i;}
        }
        int time_quantum = median(bt);
        if (bt[smallest] <= time_quantum && bt[smallest] > 0) {
          start.push_back(time);
          time += bt[smallest];
          finish.push_back(time);
          state.push_back("Id["+to_string(p[smallest])+"]");
          bt[smallest] = 0;
          counter = 1;
        } 
        else if (bt[smallest] > 0) {
          start.push_back(time);
          bt[smallest] -= time_quantum;
          time += time_quantum;
          finish.push_back(time);
          state.push_back("Id["+to_string(p[smallest])+"]");
        }
    if (bt[smallest] == 0 && counter == 1) {
      count++;
      cout<<"Process["<<p[smallest]<<"]\t\t\t"<<at[smallest]<<"\t\t"<<x[smallest]<<"\t\t"<<time - at[smallest]<<"\t\t"<<time - at[smallest] - x[smallest]<<endl;
      wait_time = wait_time + time - at[smallest] - x[smallest];
      turnaround_time = turnaround_time + time - at[smallest];
      counter = 0;
    }
    }
  awt[4] = wait_time * 1.0 / n;
  atat[4] = turnaround_time * 1.0 / n;   
    for (int i = 0; i < n; i++) {
      bt[i] = x[i];
    }
  cout<<endl;
  cout<<"Starting Time \t";
  for (int i = 0; i < start.size(); i++) {
    cout<<start[i]<<"\t";
  }
  cout<<endl;
  cout<<"Finishing Time \t";
  for (int i = 0; i < start.size(); i++) {
    cout<<finish[i]<<"\t";
  }
  cout<<endl;
  cout<<"State\t\t";
  for (int i = 0; i < start.size(); i++) {
    cout<<state[i]<<"\t";
  }
  cout<<endl;
  cout<<endl;
}

void rr() {
    start.clear();
    finish.clear();
    state.clear();
    int total = 0, x, counter = 0, time_quantum=0;
    int wait_time = 0, turnaround_time = 0, temp[100];
    x = n;
    for (int i = 0; i < n; i++) {
        temp[i] = bt[i];
    } 
  printf("\nEnter Time Quantum:\t");
  scanf("%d", & time_quantum);
  int i=0;
  printf("\nProcess ID\t\tArrivalTime\tCompletion Time\tBurst Time\t Turnaround Time\t Waiting Time\n");
  for (total = 0, i = 0; x != 0;) {
    if(at[i]<=total){
        if (temp[i] <= time_quantum && temp[i] > 0) {
        start.push_back(total);
        total = total + temp[i];
        finish.push_back(total);
        state.push_back("Id["+to_string(p[i])+"]");
          temp[i] = 0;
          counter = 1;
        } 
        else if (temp[i] > 0) {
            start.push_back(total);
            temp[i] = temp[i] - time_quantum;
            total = total + time_quantum;
            finish.push_back(total);
            state.push_back("Id["+to_string(p[i])+"]");
        }
    }
    else{
        start.push_back(total);
        finish.push_back(at[i]);
        state.push_back("Idle");
        if (temp[i] <= time_quantum && temp[i] > 0) {
          idle[2] += at[i] - total;
        start.push_back(at[i]);
          total = temp[i]+at[i];
        finish.push_back(total);
        state.push_back("Id["+to_string(p[i])+"]");
          temp[i] = 0;
          counter = 1;
        } 
        else if (temp[i] > 0) {
          idle[2] += at[i] - total;
        start.push_back(at[i]);
          total = temp[i]+at[i];
        finish.push_back(total);
        state.push_back("Id["+to_string(p[i])+"]");
          temp[i] = temp[i] - time_quantum;
          total = at[i] + time_quantum;
        }
    }

    if (temp[i] == 0 && counter == 1) {
      x--;
      cout<<"Process["<<p[i]<<"]\t\t\t"<<at[i]<<"\t\t"<<at[i]+ total - at[i]<<"\t\t"<<bt[i]<<"\t\t"<<total - at[i]<<"\t\t"<<total - at[i] - bt[i]<<endl;
      wait_time = wait_time + total - at[i] - bt[i];
      turnaround_time = turnaround_time + total - at[i];
      counter = 0;
    }
    if (i == n - 1) i = 0;
    else i++;
  }
  awt[2] = wait_time * 1.0 / n;
  atat[2] = turnaround_time * 1.0 / n;
  cout<<endl;
  cout<<"Starting Time \t";
  for (int i = 0; i < start.size(); i++) {
    cout<<start[i]<<"\t";
  }
  cout<<endl;
  cout<<"Finishing Time \t";
  for (int i = 0; i < start.size(); i++) {
    cout<<finish[i]<<"\t";
  }
  cout<<endl;
  cout<<"State\t\t";
  for (int i = 0; i < start.size(); i++) {
    cout<<state[i]<<"\t";
  }
  cout<<endl;
  cout<<endl;
}

void srtf() {
    start.clear();
    finish.clear();
    state.clear();
    int i, count = 0, time=0, smallest=0;
    vector<int> x(n);
    double avg = 0, tt = 0, end;
    for (i = 0; i < n; i++) {
      x[i] = bt[i];
    }
    printf("\nProcess ID\t\tArrivalTime\tCompletion Time\tBurst Time\t Turnaround Time\t Waiting Time\n");
    for (time = 0; count != n; time++){
        int mnn = 1e9;
        int y = 0 , mn = 1e9;
        for (i = 0; i < n; i++) {
            if (at[i] <= time && bt[i] > 0) y = 1;
        }
        if(y == 0){
            for (i = 0; i < n; i++) {
                if(bt[i] > 0)
                mn = min(mn , at[i]);
            }
            start.push_back(time);
            state.push_back("Idle");
            idle[3] += mn - time;
            time = mn;
            finish.push_back(time);
        }
        for (i = 0; i < n; i++) {
            if (at[i] <= time && bt[i] < mnn && bt[i] > 0) {mnn=bt[i];smallest = i;}
        }
        bt[smallest]--;
        start.push_back(time);
        finish.push_back(time+1);
        state.push_back("Id["+to_string(p[smallest])+"]");
        if (bt[smallest] == 0) {
            count++;
            end = time + 1;
            cout<<"Process["<<p[smallest]<<"]\t\t\t"<<at[smallest]<<"\t\t"<<at[smallest] + end - at[smallest]<<"\t\t"<<x[smallest]<<"\t\t"<<end - at[smallest]<<"\t\t"<<end - at[smallest] - x[smallest]<<endl;
            avg = avg + end - at[smallest] - x[smallest];
            tt = tt + end - at[smallest];
        }
    }
    awt[3] = avg / n;
    atat[3] = tt / n;
    for (i = 0; i < n; i++) {
      bt[i]=x[i];
    }
  cout<<endl;
  cout<<"Starting Time \t";
  for (int i = 0; i < start.size(); i++) {
    cout<<start[i]<<"\t";
  }
  cout<<endl;
  cout<<"Finishing Time \t";
  for (int i = 0; i < start.size(); i++) {
    cout<<finish[i]<<"\t";
  }
  cout<<endl;
  cout<<"State\t\t";
  for (int i = 0; i < start.size(); i++) {
    cout<<state[i]<<"\t";
  }
  cout<<endl;
  cout<<endl;
}

void display(int c) {
  printf("Average Waiting Time: %f\nAverage Turn Around Time: %f\nCPU Idleness Time: %f", awt[c - 1], atat[c - 1], idle[c-1]);
}

void sjf() {
    start.clear();
    finish.clear();
    state.clear();
    int i, count = 0, time=0, smallest=0;
    vector<int> x(n);
    double avg = 0, tt = 0, end;
    for (i = 0; i < n; i++) {
      x[i] = bt[i];
    }
    printf("\nProcess ID\t\tArrivalTime\tCompletion time\tBurst Time\t Turnaround Time\t Waiting Time\n");
    for (time = 0; count != n;){
        int mnn = 1e9;
        int y = 0 , mn = 1e9;
        for (i = 0; i < n; i++) {
            if (at[i] <= time && bt[i] > 0) y = 1;
        }
        if(y == 0){
            for (i = 0; i < n; i++) {
                if(bt[i] > 0)
                mn = min(mn , at[i]);
            }
            start.push_back(time);
            state.push_back("Idle");
            idle[1] += mn - time;
            time = mn;
            finish.push_back(time);
        }
        for (i = 0; i < n; i++) {
            if (at[i] <= time && bt[i] < mnn && bt[i] > 0) {mnn=bt[i];smallest = i;}
        }
        bt[smallest]=0;
        if (bt[smallest] == 0) {
            count++;
            start.push_back(time);
            state.push_back("Id["+to_string(p[smallest])+"]");
            end = time + x[smallest];
            time=end;
            finish.push_back(time);
            cout<<"Process["<<p[smallest]<<"]\t\t\t"<<at[smallest]<<"\t\t"<<at[smallest]+end - at[smallest]<<"\t\t"<<x[smallest]<<"\t\t"<<end - at[smallest]<<"\t\t"<<end - at[smallest] - x[smallest]<<endl;
            avg = avg + end - at[smallest] - x[smallest];
            tt = tt + end - at[smallest];
        }
    }
    awt[1] = avg / n;
    atat[1] = tt / n;
    for (i = 0; i < n; i++) {
        bt[i] = x[i];
    }
  cout<<endl;
  cout<<"Starting Time \t";
  for (int i = 0; i < start.size(); i++) {
    cout<<start[i]<<"\t";
  }
  cout<<endl;
  cout<<"Finishing Time \t";
  for (int i = 0; i < start.size(); i++) {
    cout<<finish[i]<<"\t";
  }
  cout<<endl;
  cout<<"State\t\t";
  for (int i = 0; i < start.size(); i++) {
    cout<<state[i]<<"\t";
  }
  cout<<endl;
  cout<<endl;
}

int main() {
  printf("Welcome to CPU Scheduling:\n\n");
  input();
  int c, choice;
  printf("Choice\tAlgorithm used\n1\tFCFS Algorithm\n2\tSJF Algorithm\n3\tRound robin\n4\tSRTF Algorithm\n5\tOur innovative algorithm\n");
  do {
    printf("Enter your choice from the above table :");
    scanf("%d", & c);
    switch (c) {
    case 1:
      fcfs();
      break;  
    case 2:
      sjf();
      break;
    case 3:
      rr();
      break;
    case 4:
      srtf();
      break;
    case 5:
      innovative();
      break;
    default:
      printf("Please enter choice from 1 to 5 only\n");
      break;
    }
    display(c);
    printf("\n\nEnter 1 to continue 0 to stop");
    scanf("%d", & choice);
  } while (choice == 1);
//   getch();
  int a[5][2], i;
  for (i = 0; i < 5; i++) {
    a[i][0] = awt[i];
    a[i][1] = atat[i];
  }
//   create_file_csv("schedule", a, 5, 2);
}