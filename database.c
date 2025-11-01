#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define EMAIL_MAX_LEN 18
#define USERNAME_MIN_LEN 6
#define HASH_TABLE_SIZE 100
#define LL unsigned long 



typedef struct HashNode HashNode;
typedef struct HashTable HashTable;
typedef struct DOB DOB;

struct DOB{
    int DD;
    int MM;
    int YY;
};

typedef struct{
    char* username;
    char* email;
    int age;
}Records;

typedef struct{
    Records** list;
    size_t size;
    size_t capacity;
    HashTable* lookFor;
}DB;


struct HashNode{
    char* key;
    Records* value;
    HashNode* next;
};
// static size;
struct HashTable{
    HashNode** hash_list;
    size_t size;
};
Records* find_by_username(DB* database,char* username);


// validator
// key : username
bool key_exist(DB* database,char* username){
    return find_by_username(database,username) != NULL;
}
// email validator like express validator 

bool is_email(const char* email){
    if (!email) return false;
    // @
    const char* posAt = strchr(email,'@');
    if (!posAt) return false; 

    const char* postDot = strchr(posAt,'.');
    if (!postDot) return false;

    if(strlen(email) > EMAIL_MAX_LEN ) return false;
    
    return true;
}


// hash * 2 ^ 5  + C
// (hash << 5) + C

LL hash_username(const char* username){
    LL hash = 9986532;
    int c;
    while ((c = *username++)){
        hash = ((hash << 5) + hash) + c;
    }
    return hash % HASH_TABLE_SIZE;
}

HashTable* init_hashtable(){
    HashTable* ht = malloc(sizeof(HashTable));
    ht->hash_list = calloc(HASH_TABLE_SIZE,sizeof(HashNode*));
    ht->size = 0;
    return ht;
}

void make_hash(HashTable* ht,const char* key, Records* record){
    LL index = hash_username(key);
    HashNode* hnode = malloc(sizeof(HashNode));
    hnode->key = strdup(key);
    hnode->value = record;
    hnode->next = ht->hash_list[index];
    ht->hash_list[index] = hnode;
    ht->size++;
    return;
}

// helper function for find_by_username;
Records* hash_search(HashTable* ht, char* key){
    LL index = hash_username(key);
    HashNode* hnode = ht->hash_list[index];

    while(hnode){
        if(strcmp(hnode->key,key) ==0){
            return hnode->value; // record
        }

        hnode = hnode->next;
    }
    // not found
    return NULL;
}



DB* init_DB(size_t capacity){
    DB* db = malloc(sizeof(DB));
    db->capacity = capacity;
    db->size = 0;
    db->list = (Records**) malloc(sizeof(Records*) * db->capacity);

    return db;
}

Records* add_record_to_list(char* username,char* email , int age){
    Records* record = malloc(sizeof(Records));
    record->age = age;
    record->email = strdup(email);
    record->username = strdup(username);
    
    return record;

}
void add_user(DB* database,char* username,char* email , int age){
    if(!database){
        fprintf(stderr,"DATABASE DOESN'T EXIST");
        return;
    }
    if (key_exist(database,username)){
        fprintf(stderr, "Error: username '%s' already exists.\n", username);
        return;
    }
    if (is_email(email) == false){
        fprintf(stderr, "Error: email '%s' is invalid.\n", email);
        return;
    }
    if(strlen(username) == 0 || strlen(username) < USERNAME_MIN_LEN){
        fprintf(stderr, "Error: username '%s' is invalid.\n", username);
        return;
    }


    if (database->size >= database->capacity){
        database->capacity *=2;
        database->list = (Records**) realloc(database->list, sizeof(Records*) * database->capacity);
    }

    database->list[database->size++] = add_record_to_list(username,email,age);

}
// return a whole user record 
// look for by username , email 
// O(N) version 
// deprecated both 
Records* look_by_username(DB* database,char* username){
        if(!database){
            return NULL;
        }
        for (size_t i = 0 ; i < database->size ; i++){
            if (strcmp(database->list[i]->username , username) == 0){
                return database->list[i];
            }
        }
        return NULL;
}
Records* look_by_email(DB* database,char* email){
        if(!database){
            return NULL;
        }
        for (size_t i = 0 ; i < database->size ; i++){
            if (strcmp(database->list[i]->email , email) == 0){
                return database->list[i];
            }
        }
        return NULL;
}


// return a record if user found
// using hash version  
Records* find_by_username(DB* database,char* username){
    return hash_search(database->lookFor,username);
}

Records* find_by_email(DB* database,char* email){
    return hash_search(database->lookFor,email);
}


// print the user record 
void print_user_record(Records* record){
    if(!record) return;
    printf("username:{%s},email:{%s},age:{%d} -> \n",record->username,record->email,record->age);
}
// print the whole list of user in the DB
void print_users(DB* database){
    if(!database) return;
    printf("Database contents (%zu records):\n", database->size);
    for (size_t i = 0; i < database->size; i++){
        print_user_record(database->list[i]);
    }

}
// note that username cannot be changed , its unique
void update_user_email(DB* database, char* username , char* email ){
    Records* record = find_by_username(database,username);
    if (!record) printf("Current user not found->");
    free(record->email);
    record->email = strdup(email);
    
}

void save_db_json_format(DB* database,char* filename){
    FILE* out = fopen(filename, "w");
    if (!out) {
        fprintf(stderr, "Failed to open file for writing->\n");
        return;
    }

    fprintf(out, "[\n");
    for (size_t j = 0; j < database->size; j++){
        fprintf(out, "  {\n");
        fprintf(out, "    \"username\": \"%s\",\n", database->list[j]->username);
        fprintf(out, "    \"email\": \"%s\",\n", database->list[j]->email);
        fprintf(out, "    \"age\": %d\n", database->list[j]->age);
        fprintf(out, "  }%s\n", (j < database->size - 1) ? "," : "");
    }
    fprintf(out, "]\n");
    printf("mydb exported to JSON FORMAT \n");
    fclose(out);
}


void free_db(DB* database){
    if (!database) return;
    for (size_t i = 0; i < database->size; i++){
        free(database->list[i]->username);
        free(database->list[i]->email);
        free(database->list[i]);
    }
    free(database->list);
    free(database);
}


// adding a date of birth
bool add_dob(DOB* dob,int dd,int mm,int yy){
    if (dd <= 0 || dd > 31) return false;
    if (mm <= 0 || mm > 12) return false;
    if (yy > 2026 || yy < 1900) return false;
   
    dob->DD = dd;
    dob->YY = yy;
    dob->MM = mm;

    return true;
}
// each user has a dateofbirth
void print_dob(DOB* dob){
    printf("%02d-%02d-%04d \n",dob->DD,dob->MM,dob->YY);
}

typedef enum{
    ROLE_USER,
    ROLE_MOD,
    ROL_ADMIN
}USER_ROLE;

typedef enum{
    OP_ADD_,
    OP_UPDATE_,
    OP_SAVE_DB,
    OP_LOAD_DB
}OPERATION_TYPE;

typedef enum{
    UNIQUE_KEY
}KEYS_TYPE;


/* 
TODO LIST : 
MAKE THE DATABASE ASYNC 
MAKE EACH USER WITH A ROLE
LOAD A JSON FILE 
*/


int main(void){
  
DB* db = init_DB(2);
    db->lookFor = init_hashtable();

    add_user(db, "moezgwall", "moez@Iosekva.com", 26);
    make_hash(db->lookFor, "moezgwall", db->list[0]);

    add_user(db, "zozssin", "unclebob@solid.com", 99);
    make_hash(db->lookFor, "zozin", db->list[1]);



    print_users(db);
   save_db_json_format(db,"db.json");
    Records* record_user = find_by_username(db,"alice");
    print_user_record(record_user);    
 
  free_db(db);
    return 0;
}