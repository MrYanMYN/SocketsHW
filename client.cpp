#include<iostream>
#include<fstream>
#include<stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include "db_handler.cpp"
#include "nlohmann/json.hpp"

using namespace std;

class Client_socket{
    fstream file;
    FILE* fp = NULL;
    int PORT;
    
    int general_socket_descriptor;

    struct sockaddr_in address;
    int address_length;

    char *buffer    = NULL;
    int buffer_size = 0;
    sql_handler db;

    public:
        Client_socket(){
            create_socket();
            PORT = 8050;

            address.sin_family = AF_INET;
            address.sin_port = htons( PORT );
            address_length = sizeof(address);
            if(inet_pton(AF_INET, "127.0.0.1", &address.sin_addr)<=0) { 
                cout<<"[ERROR] : Invalid address\n";
            }

            create_connection();
            
            // file.open(".//recived.jpg", ios::out | ios::trunc | ios::binary);
            // if(file.is_open()){
            //     cout<<"[LOG] : File Creted.\n";
            // }
            // else{
            //     cout<<"[ERROR] : File creation failed, Exititng.\n";
            //     exit(EXIT_FAILURE);
            // }
        }

        void create_socket(){
            if ((general_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
                perror("[ERROR] : Socket failed.\n");
                exit(EXIT_FAILURE);
            }

            // int val = 1;
            // setsockopt(general_socket_descriptor, SOL_SOCKET, SO_KEEPALIVE, &val, sizeof val);

            cout<<"[LOG] : Socket Created Successfully.\n";
        }

        void create_connection(){
            if (connect(general_socket_descriptor, (struct sockaddr *)&address, sizeof(address)) < 0) { 
                perror("[ERROR] : connection attempt failed.\n");
                exit(EXIT_FAILURE);
            }
            cout<<"[LOG] : Connection Successfull.\n";
        }

        void receive_file(){


            //receiving image

            int val_buffer = read(general_socket_descriptor, &buffer_size, sizeof(int));

            cout<<"[LOG] : Data received Buffer size "<<buffer_size<<" bytes\n";
            cout<<"[LOG] : Collecting file...\n";

            // char buffer[buffer_size];
            // memset( buffer, 0 , buffer_size*sizeof(int));
            void* void_buffer; 
            void_buffer = malloc(buffer_size);
            buffer = (char*)void_buffer;
            
            int valread = read(general_socket_descriptor , buffer, buffer_size);

            cout<<"[LOG] : Data received "<<valread<<" bytes\n";
            cout<<"[LOG] : Saving data to file.\n";
            srand(time(0));
            // unique
            string id = to_string(rand() % 100000);
            string path= "./database/photos/"+id + ".jpg";

            fp = fopen(path.c_str(), "wb");
            fwrite(buffer, buffer_size, 1, fp);

            cout<<"[LOG] : File Saved.\n";

            //get string

            char string_buffer[1024] = {};
            int string_read = read(general_socket_descriptor , string_buffer, 1024);

            cout<<"[LOG] : Data received "<<string_read<<" bytes\n";
            cout<<"[LOG] : Saving \n";

            //==========================
            //BEN INTEGRATES DATABASE AND JSON PARSING HERE
            //++++++++++++++++++++++++++
            cout<<"OWO 0 ";
            nlohmann::json j_str; 
            j_str = nlohmann::json::parse(string_buffer);
            j_str["id"] = id;
            j_str["photo_path"]= path;
            this->db.insert_data(j_str.dump());
        }
};

int main(){
    Client_socket C;
    C.receive_file();
    return 0;
}