#include "headers/utils.h"
#include "headers/cpio.h"
#include "headers/uart.h"
#include "headers/allocator.h"

void cpio_ls()
{
    char* addr = (char*) cpio_addr;
    while(1)
    {
        struct cpio_header* header = (struct cpio_header*) addr;
        unsigned int filename_size = atoi(header->c_namesize, sizeof(header->c_namesize)); 
        unsigned int content_size = atoi(header->c_filesize, sizeof(header->c_filesize));
        unsigned int header_and_filename_size = sizeof(struct cpio_header) + filename_size;

        //alignment
        // header_and_filename_size = header_and_filename_size%4==0 ? header_and_filename_size : header_and_filename_size + 4 - header_and_filename_size%4;
        // content_size = content_size%4==0 ? content_size : content_size + 4 - content_size % 4;

        align(&header_and_filename_size, 4);
        align(&content_size, 4);

        char *filename = (char*) (addr + sizeof(struct cpio_header));

        if(strcmp(filename, END_TRAIL)) break;

        display(filename);
        display(" ");

        addr += (header_and_filename_size + content_size);
    }
    display("\n");
}

void cpio_cat(char *filename)
{
    char *addr = (char*) cpio_addr;
    while(1)
    {
        struct cpio_header* header = (struct cpio_header*) addr;
        unsigned int filename_size = atoi(header->c_namesize, sizeof(header->c_namesize)); 
        unsigned int content_size = atoi(header->c_filesize, sizeof(header->c_filesize));
        unsigned int header_and_filename_size = sizeof(struct cpio_header) + filename_size;

        align(&header_and_filename_size, 4); 
        align(&content_size, 4);
        
        char *filename_ = (char*) (addr+sizeof(struct cpio_header));
        if(strcmp(filename, filename_))
        {
            char *content = (char*) (filename_ + filename_size);
            for(unsigned int i=0 ; i<content_size ; i++)
                send(content[i]);
            display("\r\n");
            return;
        }
        else if(strcmp(filename_, END_TRAIL))
        {
            display("File not found.\n");
            return;
        }

        addr += (header_and_filename_size + content_size);
    }

    display("File not found.\n");
}

char *cpio_find(char *filename)
{
    char *addr = (char*) cpio_addr;
    while(1)
    {
        struct cpio_header *header = (struct cpio_header*) addr;
        unsigned int filename_size = atoi(header->c_namesize, sizeof(header->c_namesize));
        unsigned int content_size = atoi(header->c_filesize, sizeof(header->c_filesize));

        unsigned int header_and_filename_size = sizeof(struct cpio_header) + filename_size;

        align(&header_and_filename_size, 4); 
        align(&content_size, 4);
        
        char *filename_ = (char*) (addr+sizeof(struct cpio_header));
        if(strcmp(filename, filename_))
            return filename_;
        else if(strcmp(filename_, END_TRAIL)) break;

        addr += (header_and_filename_size + content_size);
    }
    return 0;
}

void cpio_load_program(char *filename)
{
    char *addr = (char*) cpio_addr;
    while(1)
    {
        struct cpio_header* header = (struct cpio_header*) addr;
        unsigned int filename_size = atoi(header->c_namesize, sizeof(header->c_namesize)); 
        unsigned int content_size = atoi(header->c_filesize, sizeof(header->c_filesize));
        unsigned int header_and_filename_size = sizeof(struct cpio_header) + filename_size;

        align(&header_and_filename_size, 4);
        align(&content_size, 4);

        char *filename_ = (char*) (addr+sizeof(struct cpio_header));
        if(strcmp(filename, filename_))
        {
            char *content = (char*) (filename_ + filename_size);

            unsigned long sp = (unsigned long) simple_malloc(USER_STACK_SIZE);
            unsigned long j_addr = (unsigned long) content;

            char temp[8];
            bin2hex(content, temp);
            display("Find the file!\n");
            // display("Address of content: 0x"); display(temp); display("\n");
            asm("\tmov x0, xzr\n"); // 960 (debug)
            asm("\tmsr spsr_el1, x0\n");
            asm("\tmsr elr_el1, %0\n"::"r"(j_addr));
            asm("\tmsr sp_el0, %0\n"::"r"(sp+USER_STACK_SIZE));
            asm("\teret\n");

            display("ERROR. It should never execute this line.\n");
            display("Here is the function cpio_load_program in cpio.c.\n");
        }
        else if(strcmp(filename_, END_TRAIL))
        {
            display("File not found.\n");
            return;
        }

        addr += (header_and_filename_size + content_size);
    }

    display("File not found.\n");
}