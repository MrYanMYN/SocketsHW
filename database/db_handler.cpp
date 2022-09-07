#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include "nlohmann/json.hpp"
#include <fstream>
// C++ program to illustrate the
// iterators in vector
#include <iostream>
#include <vector>
 
using namespace std;

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
 {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

class sql_handler
{
 private:
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char *sql;
   bool status = false; 
   std::vector<std::string> table_columns;
   public:
   sql_handler()
   {    
       // Constructor
      if (is_file_exist("./database/data.db")==0)
      {
         std::cout<<"Creating new database"<<std::endl ;
         this->create_talbe();
      }
      //this->rc = sqlite3_open("./database/data.db", &this->db);
      this->open_db();
      if(this->rc) 
      {
         fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(this->db));
         this->status = false;
      } 
      else 
      {
         fprintf(stderr, "Opened database successfully\n");
         this->status = true;
      }
      this->table_columns.push_back(std::string("x"));
      this->table_columns.push_back(std::string("y"));
      this->table_columns.push_back(std::string("width"));
      this->table_columns.push_back(std::string("height"));
      this->table_columns.push_back(std::string("label"));
      this->table_columns.push_back(std::string("photo_path"));
      this->table_columns.push_back(std::string("id"));
      this->close_db();

   }


   bool is_file_exist(const char *fileName)
   {
    std::ifstream infile(fileName);
    std::cout<< infile.good()<<std::endl;
    return infile.good();
   }




   void insert_data(std::string json_string)
   {
      this->open_db();
      nlohmann::json json_obj;
      json_obj = nlohmann::json::parse(json_string);
      std::string sql_statement = "INSERT INTO identifications (x,y,width,height,label,photo_path,id) VALUES (";
      for (int i = 0; i< this->table_columns.size() ;i++)
      {
         sql_statement.append("'");
         sql_statement.append(json_obj[this->table_columns[i]]);
         sql_statement.append("'");
         sql_statement.append(","); 
      }
      sql_statement.pop_back();
      sql_statement.append(");");

       /* Execute SQL statement */
      this->rc = sqlite3_exec(this->db, sql_statement.c_str(), callback, 0, &this->zErrMsg);
         
      if( this->rc != SQLITE_OK )
      {
         //printf(stderr, "SQL error: %s\n", this->zErrMsg);
         std::cout<<"Failed Inserting"<<std::endl;
         //sqlite3_free(&this->zErrMsg);
      } 
      else 
      {
         std::cout<<"insetring";
      }
      this->close_db();

   }
   
   void open_db()
   {

      this->rc = sqlite3_open("./database/data.db", &this->db);

   }
   void close_db()
   {

      sqlite3_close(this->db);

   }
   void create_talbe()
   {
         /* Create SQL statement */
      this->open_db();
      std::string table_create_sql =  "CREATE TABLE identifications (" \
      "x INT NOT NULL, " \
      "y INT NOT NULL," \
      "width INT NOT NULL," \
      "height INT NOT NULL," \
      "label TEXT NOT NULL," \
      "photo_path TEXT NOT NULL,"
      "id int NOT NULL UNIQUE PRIMARY KEY" \
      ");" ;
     
      this->rc = sqlite3_exec(this->db,table_create_sql.c_str(), callback, 0, &this->zErrMsg);
         
      if( this->rc != SQLITE_OK )
      {
         fprintf(stderr, "SQL error: %s\n", this->zErrMsg);
         sqlite3_free(this->zErrMsg);
      } 
      else 
      {
         fprintf(stdout, "Table created successfully\n");
      }
      std::cout<<table_create_sql<<std::endl;
      this->close_db();
   }
   void remove_rows(std::string id)
   {
      //DELETE FROM identifications WHERE id==12546;
      this->open_db();
      std::string remove_sql =  "DELETE FROM identifications WHERE id=="+id;
      this->rc = sqlite3_exec(this->db,remove_sql.c_str(), callback, 0, &this->zErrMsg);
            
      if( this->rc != SQLITE_OK )
      {
         fprintf(stderr, "SQL error: %s\n", this->zErrMsg);
         sqlite3_free(this->zErrMsg);
      } 
      else 
      {
         fprintf(stdout, "Removed successfully\n");
      }
      std::cout<<remove_sql<<std::endl;

      this->close_db();
   }
};



int main(int argc, char* argv[]) 
{
   sql_handler db_handler;
   std::ifstream ifs("j.json");
   nlohmann::json  jf = nlohmann::json::parse(ifs);
   db_handler.insert_data(jf.dump());
}  