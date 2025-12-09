#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "_defs.h"
#include "_null.h"

#define MAX_FILENAME_SIZE 13
#define CHUNK_SIZE 512

const char* intro_msg = "[[ XOR <=512B-keys File Encryption & Decryption Program ]]";
const char* info_msg = "[[ File names have to be less than 13 characters, including extension ]]";
const char* data_msg = "--> Data file name: ";
const char* key_msg = "--> Key file name: ";
const char* res_msg = "--> Result file name: ";
const char* err_msg = "[[ ERR: ";
const char* err_end_msg = " is missing! ]]";
const char* err_open_msg = "[[ ERR: Could not open a file! ]]";
const char* err_create_msg = "[[ ERR: Could not create the result file! ]]";
const char* success_msg = "[[ DONE! ]]";

char data[MAX_FILENAME_SIZE] = { 0 };
char key[MAX_FILENAME_SIZE] = { 0 };
char res[MAX_FILENAME_SIZE] = { 0 };

unsigned char data_chunk[CHUNK_SIZE] = { 0 };
unsigned char key_content[CHUNK_SIZE] = { 0 };
unsigned char res_chunk[CHUNK_SIZE] = { 0 };

short int key_len = 0;
short int bytes_read = 0;

FILE* f_data = NULL;
FILE* f_key = NULL;
FILE* f_res = NULL;

short int i;

int main(void)
{
    printf("%s\n", intro_msg);
    printf("%s\n\n", info_msg);

    printf("%s", data_msg);
    scanf("%12s", data);
    printf("%s", key_msg);
    scanf("%12s", key);
    printf("%s", res_msg);
    scanf("%12s", res);

    f_data = fopen(data, "rb");
    if (!f_data) {
        printf("%s%s%s\n", err_msg, data, err_end_msg);
        return 1;
    }

    f_key = fopen(key, "rb");
    if (!f_key) {
        printf("%s%s%s\n", err_msg, key, err_end_msg);
        fclose(f_data);
        return 1;
    }

    key_len = fread(key_content, 1, CHUNK_SIZE, f_key);
    fclose(f_key);

    if (key_len == 0) {
        printf("%s\n", err_open_msg);
        fclose(f_data);
        return 1;
    }

    f_res = fopen(res, "wb");
    if (!f_res) {
        printf("%s\n", err_create_msg);
        fclose(f_data);
        return 1;
    }

    while ((bytes_read = fread(data_chunk, 1, CHUNK_SIZE, f_data)) > 0) {
        for (i = 0; i < bytes_read; i++) {
            res_chunk[i] = data_chunk[i] ^ key_content[i % key_len];
        }
        fwrite(res_chunk, 1, bytes_read, f_res);
    }

    fclose(f_data);
    fclose(f_res);

    printf("\n\n%s\n", success_msg);
    return 0;
}