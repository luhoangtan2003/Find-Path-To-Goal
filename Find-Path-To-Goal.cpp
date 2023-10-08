#include<bits/stdc++.h>                 // Thư viện chuẩn của C++ bao gồm tất cả các thư viện cơ bản
using namespace std;

#define MAX_VERTICES 5                  // Định nghĩa hằng số MAX_VERTICES với giá trị là 5, tức là số đỉnh tối đa của đồ thị
#define MAX_LENGTH 1000                 // Định nghĩa hằng số MAX_LENGTH với giá trị là 1000, tức là độ dài tối đa của danh sách

struct Vertice{                         // Định nghĩa cấu trúc cho một đỉnh
    int Neighbor[MAX_VERTICES];         // Mảng lưu thông tin về các đỉnh kề với đỉnh hiện tại
    int H;                              // Hàm heuristic của đỉnh
};

struct Graph{                           // Định nghĩa cấu trúc cho đồ thị
    int Num_Vertices;                   // Số đỉnh của đồ thị
    Vertice Vertices[MAX_VERTICES];     // Mảng lưu các đỉnh của đồ thị
};

void Init_Graph(int Num, Graph *G){     // Hàm khởi tạo đồ thị
    G->Num_Vertices = Num;              // Gán số đỉnh của đồ thị
    for(int i=0;i<Num;i++){             // Duyệt qua các đỉnh của đồ thị
        for(int j=0;j<Num;j++){
            G->Vertices[i].Neighbor[j] = 0;    // Ban đầu các đỉnh không có kết nối với nhau
            G->Vertices[i].H = 0;              // Ban đầu hàm heuristic của các đỉnh đều là 0
        }
    }
}

const char Name[] = {'A','B','C','D','G'};         // Tên các đỉnh

struct State{                           // Định nghĩa cấu trúc cho trạng thái của việc tìm đường đi
    int Vertex;                         // Đỉnh hiện tại
};

void Print_State(State State){           // Hàm in ra trạng thái (tên đỉnh)
    printf("%c ", Name[State.Vertex]);   // In ra tên của đỉnh hiện tại theo mảng Name
}

int Compare_States( State S1, State S2){ // Hàm so sánh 2 trạng thái
    return S1.Vertex == S2.Vertex;      // Trả về 1 nếu 2 trạng thái giống nhau và trả về 0 nếu ngược lại (so sánh theo thuộc tính Vertex)
}

struct Node{                            // Định nghĩa cấu trúc cho 1 node trong cây tìm kiếm
    State State;                        // Trạng thái hiện tại của node đó
    Node *Dad;                          // Node cha của node hiện tại
    int F,G,H;                          // Các giá trị tương ứng với thuật toán A*
};

struct List{                            // Định nghĩa cấu trúc cho 1 danh sách
    Node *Element[MAX_LENGTH];          // Mảng lưu các node
    int Size;                           // Số lượng node hiện tại của danh sách
};

void Makenulllist(List *List){           // Hàm khởi tạo danh sách
    List->Size = 0;                      // Danh sách trống (kích thước bằng 0)
}
int Emptylist(List List){               // Hàm kiểm tra danh sách có rỗng hay không
    return List.Size==0;                // Trả về 1 nếu danh sách rỗng và trả về 0 nếu ngược lại (kiểm tra theo thuộc tính Size)
}
int Fulllist(List List){                // Hàm kiểm tra danh sách đã đầy chưa
    return List.Size == MAX_LENGTH;     // Trả về 1 nếu danh sách đầy và trả về 0 nếu ngược lại (so sánh thuộc tính Size với hằng số MAX_LENGTH)
}

Node *Element_At(List List, int Position){  // Hàm trả về node ở vị trí thứ Position trong danh sách
    return List.Element[Position-1];    // Trả về node ở vị trí thứ Position-1 trong mảng Element
}

void Pushlist(Node *X,int Position, List *List){  // Hàm thêm node X vào danh sách ở vị trí Position
    if(!Fulllist(*List)){                       // Nếu danh sách chưa đầy
        if(Position<1||Position>List->Size+1){   // Nếu vị trí không đúng
            printf("Position is impossible to add.\n"); // In ra thông báo lỗi
        }else{
            for(int Q=List->Size;Q>=Position;Q--){    // Dời các node từ vị trí Position về sau về phía sau 1 bậc
                List->Element[Q] = List->Element[Q-1];
            }
            List->Element[Position-1] = X;      // Thêm node vào vị trí đúng
            List->Size++;                        // Tăng kích thước của danh sách lên 1
        }
    }else{                                      // Nếu danh sách đầy
        printf("List is full of the elements.\n"); // In ra thông báo lỗi
    }
}

void Deletelist(int Position, List *List){     // Hàm xóa node ở vị trí Position từ danh sách
    if(Emptylist(*List)){                       // Nếu danh sách rỗng
        printf("List is empty,cannot delete.\n"); // In ra thông báo lỗi
    }else if(Position<1||Position>List->Size){ // Nếu vị trí xóa không đúng
        printf("Position is impossible to delete.\n");   // In ra thông báo lỗi
    }else{
        for(int i=Position-1;i<List->Size;i++){           // Dời các node từ vị trí Position về sau lên phía trước 1 bậc
            List->Element[i] = List->Element[i+1];
        }
        List->Size--;                                   // Giảm số lượng node trong danh sách
    }
}

Node *Find_State(State State, List List, int *Pos){ // Hàm tìm kiếm một trạng thái trong danh sách
    for( int i=1;i<=List.Size;i++){ // Duyệt qua từng node trong danh sách
        if(Compare_States(Element_At(List,i)->State,State)){   // Nếu tìm thấy trạng thái giống nhau
            *Pos = i;   // Gán vị trí của node chứa trạng thái đó vào biến Pos
            return Element_At(List,i); // Trả về node đó
        }
    }
    return NULL;    // Nếu không tìm thấy trạng thái, trả về NULL
}

int Goal_Check(State S, State Goal){    // Hàm kiểm tra đã tới được đích hay chưa
    return Compare_States(S,Goal);      // Trả về 1 nếu đã tới được đích và trả về 0 nếu ngược lại (so sánh theo thuộc tính Vertex)
}

void Sort(List *List){                  // Hàm sắp xếp danh sách theo giá trị G tăng dần
    for(int i=0;i<List->Size-1;i++){    // Duyệt qua từng node trong danh sách
        for(int j=i+1;j<List->Size;j++){// Duyệt qua từng node còn lại trong danh sách
            if(List->Element[i]->G>List->Element[j]->G){ // Nếu giá trị G của node i lớn hơn giá trị G của node j
                Node *Temp = List->Element[i];   // Hoán đổi 2 node cho nhau
                List->Element[i] = List->Element[j];
                List->Element[j] = Temp;
            }
        }
    }
}

Node *A_Star(Graph G, State S, State Goal){  // Hàm tìm kiếm đường đi từ đỉnh S đến đỉnh Goal trên đồ thị G bằng thuật toán A*
    List Open_A_Star;                       // Danh sách các node chưa được xét
    List Closed_A_Star;                     // Danh sách các node đã được xét
    Makenulllist(&Open_A_Star);             // Khởi tạo danh sách Open rỗng
    Makenulllist(&Closed_A_Star);           // Khởi tạo danh sách Close rỗng
    Node *Root = (Node*)malloc(sizeof(Node));   // Tạo ra node gốc
    Root->State = S;                        // Đặt trạng thái của node gốc bằng trạng thái đỉnh S
    Root->Dad = NULL;                       // Node gốc không có node cha
    Root->G = 0;                            // Chi phí đi từ node gốc đến node gốc là 0
    Root->H = G.Vertices[S.Vertex].H;      // Hàm heuristic tại node gốc bằng hàm heuristic của đỉnh S
    Root->F = Root->G + Root->H;            // Giá trị F tại node gốc
    Pushlist(Root,Open_A_Star.Size+1,&Open_A_Star);  // Thêm node gốc vào danh sách Open
    while(!Emptylist(Open_A_Star)){         // Khi danh sách Open còn node chưa được xét
        Node *NODE = Element_At(Open_A_Star,1);  // Lấy ra node với giá trị G nhỏ nhất trong danh sách Open
        Deletelist(1,&Open_A_Star);         // Xóa node đó khỏi danh sách Open
        Pushlist(NODE,Closed_A_Star.Size+1,&Closed_A_Star); // Thêm node đó vào danh sách Close
        if(Goal_Check(NODE->State,Goal)){   // Nếu đã tìm thấy đường đi đến đích
            return NODE;                    // Trả về node đích
        }
        int Option;
        for(Option=0;Option<G.Num_Vertices;Option++){    // Duyệt qua các đỉnh kề với đỉnh hiện tại
            if(G.Vertices[NODE->State.Vertex].Neighbor[Option]>0){
                Node *Newnode = (Node*)malloc(sizeof(Node));   // Tạo ra node mới
                Newnode->State.Vertex = Option;         // Đặt trạng thái của node bằng đỉnh kề hiện tại
                Newnode->Dad = NODE;                    // Node cha của node mới là node hiện tại
                Newnode->G = NODE->G + G.Vertices[NODE->State.Vertex].Neighbor[Option]; // Tính toán giá trị G tại node mới
                Newnode->H = G.Vertices[Option].H;      // Tính toán giá trị H tại node mới
                Newnode->F = Newnode->G + Newnode->H;   // Tính toán giá trị F tại node mới
                int Pos_Open, Pos_Close;                // Khai báo biến để lưu vị trí tìm kiếm node trong danh sách
                Node *Existed_Open = Find_State(Newnode->State,Open_A_Star,&Pos_Open);  // Tìm kiếm node trong danh sách Open
                Node *Existed_Close = Find_State(Newnode->State,Closed_A_Star,&Pos_Close);  // Tìm kiếm node trong danh sách Close
                if(Existed_Open == NULL && Existed_Close == NULL){        // Nếu node đó không có trong danh sách nào
                    Pushlist(Newnode,Open_A_Star.Size+1,&Open_A_Star);    // Thêm node đó vào danh sách Open
                }else if(Existed_Open != NULL && Existed_Open->G > Newnode->G){ // Nếu node đó đã có trong danh sách Open và có giá trị G lớn hơn
                    Deletelist(Pos_Open,&Open_A_Star);                  // Xóa node đó khỏi danh sách Open
                    Pushlist(Newnode,Pos_Open,&Open_A_Star);            // Thêm node mới vào vị trí đúng trong danh sách Open
                }else if(Existed_Close != NULL && Existed_Close->G > Newnode->G){ // Nếu node đó đã có trong danh sách Close và có giá trị G lớn hơn
                    Deletelist(Pos_Close,&Closed_A_Star);                // Xóa node đó khỏi danh sách Close
                    Pushlist(Newnode,Open_A_Star.Size+1,&Open_A_Star);    // Thêm node đó vào danh sách Open
                }
            }
        }
        Sort(&Open_A_Star);                 // Sắp xếp lại danh sách Open theo giá trị G tăng dần
    }
    return NULL;                            // Nếu không tìm thấy đường đi đến đích, trả về NULL
}

void Print_WayToGetGoal(Node *N){           // Hàm in ra đường đi từ đỉnh đầu đến đỉnh đích
    List List;                              // Khai báo danh sách để lưu các node trên đường đi
    Makenulllist(&List);                    // Khởi tạo danh sách rỗng
    while(N->Dad!=NULL){                        // Duyệt qua các node cha của node đích
        Pushlist(N,List.Size+1,&List);          // Thêm node cha vào danh sách
        N = N->Dad;                             // Xét tiếp node cha
    }
    Pushlist(N,List.Size+1,&List);              // Thêm node đích vào danh sách
    printf("Đường đi từ đầu đến đích: ");
    for(int i=List.Size;i>0;i--){               // In tên của các đỉnh trong đường đi
        Print_State(Element_At(List,i)->State);
    }
}

int main(int argc, char const *argv[]){
    Graph Graph;                               // Khai báo biến Graph để lưu trữ thông tin về đồ thị
    freopen("D:\\Github\\Find-Path-To-Goal\\Find-Path-To-Goal.txt","r",stdin);      // Mở file chứa dữ liệu đầu vào
    Init_Graph(MAX_VERTICES,&Graph);    // Khởi tạo đồ thị
    for(int i=0;i<MAX_VERTICES;i++){    // Đọc dữ liệu đầu vào
        int Heuristic;
        scanf("%d",&Heuristic);
        Graph.Vertices[i].H = Heuristic;
        for(int j=0;j<MAX_VERTICES;j++){
            scanf("%d",&Heuristic);
            Graph.Vertices[i].Neighbor[j] = Heuristic;
        }
    }
    State A,G;
    A.Vertex = 0;                       // Đỉnh bắt đầu là đỉnh A
    G.Vertex = 4;                       // Đỉnh đích là đỉnh G
    Node *Result = A_Star(Graph,A,G);   // Tìm đường đi từ A đến G bằng thuật toán A*
    if(Result != NULL){
        Print_WayToGetGoal(Result);    // Nếu tìm thấy đường đi, in ra đường đi đó
    }else{
        printf("Không tìm thấy đường đi từ A đến G.\n");     // In ra thông báo nếu không tìm thấy đường đi
    }
}