#include<iostream>
#include<fstream>
#include<stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "nlohmann/json.hpp"
using namespace std;

class Server_socket{
    fstream file;
    
    int PORT;
    
    int general_socket_descriptor;
    int new_socket_descriptor;

    struct sockaddr_in address;
    int address_length;
    nlohmann::json j_str = nlohmann::json::parse(std::ifstream("j.json"));
    int buffer_size = 0;
    char *buffer     = NULL;

    public:
        Server_socket(){
            create_socket();
            PORT = 8050;

            address.sin_family = AF_INET; 
            address.sin_addr.s_addr = INADDR_ANY; 
            address.sin_port = htons( PORT );
            address_length = sizeof(address);

            bind_socket();
            set_listen_set();
            accept_connection();
            read_image_file("./image.jpg", &buffer, &buffer_size);


            // file.open(".//Data//Server//server_png.png", ios::in | ios::binary);
            // if(file.is_open()){
            //     cout<<"[LOG] : File is ready to Transmit.\n";
            // }
            // else{
            //     cout<<"[ERROR] : File loading failed, Exititng.\n";
            //     exit(EXIT_FAILURE);
            // }
        }

        void create_socket(){
            if ((general_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
                perror("[ERROR] : Socket failed");
                exit(EXIT_FAILURE);
            }
            cout<<"[LOG] : Socket Created Successfully.\n";
        }

        void bind_socket(){
            if (bind(general_socket_descriptor, (struct sockaddr *)&address, sizeof(address))<0) {
                perror("[ERROR] : Bind failed");
                exit(EXIT_FAILURE);
            }
            cout<<"[LOG] : Bind Successful.\n";
        }

        void set_listen_set(){
            if (listen(general_socket_descriptor, 3) < 0) {
                perror("[ERROR] : Listen");
                exit(EXIT_FAILURE);
            }
            cout<<"[LOG] : Socket in Listen State (Max Connection Queue: 3)\n";
        }

        void accept_connection(){
            if ((new_socket_descriptor = accept(general_socket_descriptor, (struct sockaddr *)&address, (socklen_t*)&address_length))<0) { 
                perror("[ERROR] : Accept");
                exit(EXIT_FAILURE);
            }
            cout<<"[LOG] : Connected to Client.\n";
        }

        void transmit_file(){



            //Sending Image

            cout<<"[LOG] : Transmission Data Size "<<buffer_size<<" Bytes.\n";

            cout<<"[LOG] : Sending File Size...";

            int buffer_sent = send(new_socket_descriptor, &buffer_size, sizeof(int), 0);

            cout<<"[LOG] : Transmitted Data Sized header "<<buffer_sent<<" Bytes. \n";

            cout<<"[LOG] : Sending...\n";
            
            int bytes_sent = send(new_socket_descriptor , buffer , buffer_size , 0);
            cout<<"[LOG] : Transmitted Data Size "<<bytes_sent<<" Bytes. \n";

            cout<<"[LOG] : File Transfer Complete.\n";
            
            // Sending Text
            /// should be loop to enter correctly the json string 
            std::string contents =  j_str.dump();

            cout<<"[LOG] : Transmission Data String "<<contents.length()<<" Bytes.\n";

            cout<<"[LOG] : Sending...\n";

            int string_sent = send(new_socket_descriptor , contents.c_str() , contents.length() , 0 );

            cout<<"[LOG] : String Transfer Complete.\n";
        }

        
        int read_image_file(char *file_name, char **buffer, int *sizeof_buffer) {
                int c, i;
                int char_read = 0;
                FILE* fp = fopen(file_name, "rb");

                if (fp == NULL) {
                        fprintf(stderr, "\t Can't open file : %s", file_name);
                        return -1;
                }
                
                fseek(fp, 0L, SEEK_END);
                *sizeof_buffer = ftell(fp);
                fseek(fp, 0L, SEEK_SET);
                *buffer = (char *)malloc(*sizeof_buffer);
                fread(*buffer, *sizeof_buffer, 1, fp);
                return 0;
        }

};

int main(){
    Server_socket S;
    S.transmit_file();
    return 0;
}