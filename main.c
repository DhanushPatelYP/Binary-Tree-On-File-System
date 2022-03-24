#include <stdio.h>
#include <stdlib.h>
#include "main.h"
int COUNT = 0;
int DELETE=0;
// create a file with the filename and initialize the header with tree_t structure
// if the file already exists, just return the file ptr
FILE *init_tree(const char *filename)
{
    FILE *fp;
    fp = fopen(filename, "w+");
    if (fp == NULL)
    {
        exit(1);
    }
    else
    {
        tree_t header;
        header.free_head = 0;
        header.root = -1;
        fwrite(&header, sizeof(tree_t), 1, fp);
    }
    fseek(fp,0,SEEK_SET);
    return fp;
}

// close the file pointed by fp
void close_tree(FILE *fp)
{
    fclose(fp);
}


void inorder(node_t x,FILE* fp)
{

    if((x.left_offset == -1) && (x.right_offset == -1))
    {
        printf("%d ",x.key);
        return ;
    }
    if(x.left_offset != -1)
    {
        node_t q;
        fseek(fp,0,SEEK_SET);
        fseek(fp,sizeof(tree_t)+sizeof(node_t)* x.left_offset,SEEK_SET);
        fread(&q,sizeof(node_t),1,fp);
        inorder(q,fp);
            
    }
    printf("%d ",x.key);

    if(x.right_offset != -1)
    {
        node_t p;
        fseek(fp,0,SEEK_SET);
        fseek(fp,sizeof(tree_t)+sizeof(node_t)* x.right_offset,SEEK_SET);
        fread(&p,sizeof(node_t),1,fp);
        inorder(p,fp);
    }
}


// Space separated keys. Last key followed by '\n'
void display_inorder(FILE *fp)
{
    tree_t h;
    fseek(fp,0,SEEK_SET);
    fread(&h,sizeof(tree_t),1,fp);
    if(COUNT == 0)
    {
        printf("\n");
        return;
    }
    else
    {
        node_t x;
        fseek(fp,0,SEEK_SET);
        fseek(fp,sizeof(tree_t)+sizeof(node_t)*h.root,SEEK_SET);
        fread(&x,sizeof(node_t),1,fp);
        inorder(x,fp);
    }
}


void preorder(node_t x,FILE* fp)
{
    if((x.left_offset == -1) && (x.right_offset == -1))
    {
        printf("%d ",x.key);
        return;
    }
    printf("%d ",x.key);
    if(x.left_offset != -1)
    {
        node_t q;
        fseek(fp,0,SEEK_SET);
        fseek(fp,sizeof(tree_t)+sizeof(node_t)* x.left_offset,SEEK_SET);
        fread(&q,sizeof(node_t),1,fp);
        preorder(q,fp);
            
    }
    if(x.right_offset != -1)
    {
        node_t p;
        fseek(fp,0,SEEK_SET);
        fseek(fp,sizeof(tree_t)+sizeof(node_t)* x.right_offset,SEEK_SET);
        fread(&p,sizeof(node_t),1,fp);
        preorder(p,fp);
    }

}


// Space separated keys. Last key followed by '\n'
void display_preorder(FILE *fp)
{
    tree_t h;
    fseek(fp,0,SEEK_SET);
    fread(&h,sizeof(tree_t),1,fp);
    if(COUNT == 0)
    {
        printf("\n");
        return;
    }
    else
    {
        node_t x;
        fseek(fp,0,SEEK_SET);
        fseek(fp,sizeof(tree_t)+sizeof(node_t)*h.root,SEEK_SET);
        fread(&x,sizeof(node_t),1,fp);
        preorder(x,fp);
    }
}


// insert the key into the tree in the file
// if the key already exists just return
void insert_key(int key, FILE *fp)
{
    node_t temp;
    temp.key = key;
    temp.left_offset = temp.right_offset = -1;
    tree_t x;
    fseek(fp, 0, SEEK_SET);
    fread(&x, sizeof(tree_t), 1, fp);

    if (x.root == -1)
    {
        COUNT++;
        fseek(fp, 0, SEEK_SET);
        x.free_head = 1;
        x.root = 0;
        fwrite(&x, sizeof(tree_t), 1, fp);
        fwrite(&temp, sizeof(node_t), 1, fp);
        
    }
    else
    {
        node_t curr;
        fseek(fp, 0, SEEK_SET);
        fseek(fp, sizeof(tree_t) + (sizeof(node_t) * x.root), SEEK_SET);
        fread(&curr, sizeof(node_t), 1, fp);
        int i=1;
        while(i)
        {
            if (temp.key == curr.key)
            {
                return;
            }
            else if (temp.key > curr.key)
            {
                if (curr.right_offset == -1)
                {
                    COUNT++;

                    int free;
                    free = x.free_head;
                    fseek(fp, -(sizeof(node_t)), SEEK_CUR);
                    node_t q;
                    q.key = curr.key;
                    q.left_offset = curr.left_offset;
                    q.right_offset = x.free_head;
                    fwrite(&q, sizeof(node_t), 1, fp);
                    fseek(fp, 0, SEEK_SET);
                    fseek(fp, sizeof(tree_t) + (sizeof(node_t) * x.free_head), SEEK_SET);
                    if(DELETE == 0)
                    {
                        x.free_head = free + 1;
                    }
                    else
                    {
                        node_t p;
                        fread(&p,sizeof(node_t),1,fp);
                        x.free_head = p.left_offset;
                        DELETE--;
                    }
                    fseek(fp, 0, SEEK_SET);
                    fseek(fp, sizeof(tree_t) + (sizeof(node_t) * free), SEEK_SET);
                    fwrite(&temp, sizeof(node_t), 1, fp);
                    fseek(fp, 0, SEEK_SET);
                    fwrite(&x, sizeof(tree_t), 1, fp);
                    i=0;
                }
                else
                {
                    fseek(fp, 0, SEEK_SET);
                    fseek(fp, sizeof(tree_t) + (sizeof(node_t) * curr.right_offset), SEEK_CUR);
                    fread(&curr, sizeof(node_t), 1, fp);
                }
            }
            else if(temp.key < curr.key)
            {
            
                if(curr.left_offset == -1)
                {
                    COUNT++;
                    int free;
                    free = x.free_head;
                    fseek(fp,-(sizeof(node_t)),SEEK_CUR);
                    node_t q;
                    q.key=curr.key;
                    q.right_offset=curr.right_offset;
                    q.left_offset=x.free_head;
                    fwrite(&q,sizeof(node_t),1,fp);
                    fseek(fp,0,SEEK_SET);
                    fseek(fp,sizeof(tree_t)+(sizeof(node_t)*x.free_head),SEEK_SET);
                    if(DELETE == 0)
                    {
                        x.free_head = free + 1;
                    }
                    else
                    {
                        node_t p;
                        fread(&p,sizeof(node_t),1,fp);
                        x.free_head = p.left_offset;
                        DELETE--;
                    }
                    fseek(fp, 0, SEEK_SET);
                    fseek(fp, sizeof(tree_t) + (sizeof(node_t) * free), SEEK_SET);
                    fwrite(&temp,sizeof(node_t),1,fp);
                    fseek(fp,0,SEEK_SET);

                    fwrite(&x,sizeof(tree_t),1,fp);
                    i=0;
            
                }
                else
                {
                    fseek(fp,0,SEEK_SET);
                    fseek(fp,sizeof(tree_t)+(sizeof(node_t)*curr.left_offset),SEEK_SET);
                    fread(&curr,sizeof(node_t),1,fp);

                }
            }
        } 
    }
}

// delete the key from the tree in the file
// the key may or may not exist
void delete_key(int key, FILE *fp)
{
    int delval;
    node_t del;
    node_t prev;
    tree_t head;
    fseek(fp,0,SEEK_SET);
    fread(&head,sizeof(tree_t),1,fp);
    fseek(fp,0,SEEK_SET);
    fseek(fp,sizeof(tree_t)+sizeof(node_t)*head.root,SEEK_SET);
    fread(&del,sizeof(node_t),1,fp);
    
    int i=0;
    while(1)
    {
        if(i == COUNT)
        {
            break;
        }
        if(key < del.key)
        {
            int val;
            prev.left_offset = del.left_offset;
            prev.key = del.key;
            prev.right_offset = del.right_offset;
            fseek(fp,0,SEEK_SET);
            val=del.left_offset;
            delval=del.left_offset;
            fseek(fp,0,SEEK_SET);
            fseek(fp,sizeof(tree_t)+sizeof(node_t)*val,SEEK_SET);
            fread(&del,sizeof(node_t),1,fp);
        }

        else if(key > del.key)
        {
            int val;
            prev.left_offset = del.left_offset;
            prev.key = del.key;
            prev.right_offset = del.right_offset;
            fseek(fp,0,SEEK_SET);
            val=del.right_offset;
            delval=del.right_offset;
            fseek(fp,0,SEEK_SET);
            fseek(fp,sizeof(tree_t)+sizeof(node_t)*val,SEEK_SET);
            fread(&del,sizeof(node_t),1,fp);
        }
        i+=1;
    }

    // if the deleteing node does not exists

    if (del.key != key)
    {
        return;
    }

    // if it is a leaf node

    else if((del.left_offset == -1) && (del.right_offset== -1))
    {
        DELETE +=1;
        if(del.key < prev.key)
        {
            del.left_offset = head.free_head;
            del.right_offset = -1;
            fseek(fp,-sizeof(node_t),SEEK_CUR);
            fwrite(&del,sizeof(node_t),1,fp);

            head.free_head = prev.left_offset;
            fseek(fp,0,SEEK_SET);
            fwrite(&head,sizeof(tree_t),1,fp);

            prev.left_offset = -1;
            node_t find;
            fseek(fp,0,SEEK_SET);
            fseek(fp,sizeof(tree_t)+sizeof(node_t)*head.root,SEEK_SET);
            fread(&find,sizeof(node_t),1,fp);
            while(find.key != prev.key)
            {
                    if(prev.key < find.key)
                    {
                        int val;
                        val=find.left_offset;
                        fseek(fp,0,SEEK_SET);
                        fseek(fp,sizeof(tree_t)+sizeof(node_t)*val,SEEK_SET);
                        fread(&find,sizeof(node_t),1,fp);
                    }

                    else if(prev.key > find.key)
                    {
                        int val;
                        val=find.right_offset;
                        fseek(fp,0,SEEK_SET);
                        fseek(fp,sizeof(tree_t)+sizeof(node_t)*val,SEEK_SET);
                        fread(&find,sizeof(node_t),1,fp);
                    }
            }

            fseek(fp,-sizeof(node_t),SEEK_CUR);
            fwrite(&prev,sizeof(node_t),1,fp);
        }
        else if(del.key > prev.key)
        {
            del.left_offset = head.free_head;
            del.right_offset = -1;
            fseek(fp,-sizeof(node_t),SEEK_CUR);
            fwrite(&del,sizeof(node_t),1,fp);

            head.free_head = prev.right_offset;
            fseek(fp,0,SEEK_SET);
            fwrite(&head,sizeof(tree_t),1,fp);

            prev.right_offset = -1;
            node_t find;
            fseek(fp,0,SEEK_SET);
            fseek(fp,sizeof(tree_t)+sizeof(node_t)*head.root,SEEK_SET);
            fread(&find,sizeof(node_t),1,fp);
            while(find.key != prev.key)
            {
                    if(prev.key < find.key)
                    {
                        int val;
                        val=find.left_offset;
                        fseek(fp,0,SEEK_SET);
                        fseek(fp,sizeof(tree_t)+sizeof(node_t)*val,SEEK_SET);
                        fread(&find,sizeof(node_t),1,fp);
                    }

                    else if(prev.key > find.key)
                    {
                        int val;
                        val=find.right_offset;
                        fseek(fp,0,SEEK_SET);
                        fseek(fp,sizeof(tree_t)+sizeof(node_t)*val,SEEK_SET);
                        fread(&find,sizeof(node_t),1,fp);
                    }
            }

            fseek(fp,-sizeof(node_t),SEEK_CUR);
            fwrite(&prev,sizeof(node_t),1,fp);
        }
    }
    // if it has has one node 

    else if(((del.left_offset == -1) && (del.right_offset != -1)) || (del.left_offset != -1) && (del.right_offset == -1))
    {
        DELETE +=1;
        int val;
        if((del.left_offset == -1) && (del.right_offset != -1))
        {
            val=del.right_offset;
        }
        else{
            val=del.left_offset;
        }
        if(del.key < prev.key)
        {
            del.left_offset = head.free_head;
            del.right_offset = -1;
            fseek(fp,-sizeof(node_t),SEEK_CUR);
            fwrite(&del,sizeof(node_t),1,fp);

            head.free_head = prev.left_offset;
            fseek(fp,0,SEEK_SET);
            fwrite(&head,sizeof(tree_t),1,fp);

            // prev.left_offset = -1;
            node_t find;
            fseek(fp,0,SEEK_SET);
            fseek(fp,sizeof(tree_t)+sizeof(node_t)*head.root,SEEK_SET);
            fread(&find,sizeof(node_t),1,fp);
            while(find.key != prev.key)
            {
                    if(prev.key < find.key)
                    {
                        int val;
                        val=find.left_offset;
                        fseek(fp,0,SEEK_SET);
                        fseek(fp,sizeof(tree_t)+sizeof(node_t)*val,SEEK_SET);
                        fread(&find,sizeof(node_t),1,fp);
                    }

                    else if(prev.key > find.key)
                    {
                        int val;
                        val=find.right_offset;
                        fseek(fp,0,SEEK_SET);
                        fseek(fp,sizeof(tree_t)+sizeof(node_t)*val,SEEK_SET);
                        fread(&find,sizeof(node_t),1,fp);
                    }
            }
            prev.left_offset=val;
            fseek(fp,-sizeof(node_t),SEEK_CUR);
            fwrite(&prev,sizeof(node_t),1,fp);
        }
        else if(del.key > prev.key)
        {
            del.left_offset = head.free_head;
            del.right_offset =-1;
            fseek(fp,-sizeof(node_t),SEEK_CUR);
            fwrite(&del,sizeof(node_t),1,fp);

            head.free_head = prev.right_offset;
            fseek(fp,0,SEEK_SET);
            fwrite(&head,sizeof(tree_t),1,fp);

            prev.right_offset = -1;
            node_t find;
            fseek(fp,0,SEEK_SET);
            fseek(fp,sizeof(tree_t)+sizeof(node_t)*head.root,SEEK_SET);
            fread(&find,sizeof(node_t),1,fp);
            while(find.key != prev.key)
            {
                    if(prev.key < find.key)
                    {
                        int val;
                        val=find.left_offset;
                        fseek(fp,0,SEEK_SET);
                        fseek(fp,sizeof(tree_t)+sizeof(node_t)*val,SEEK_SET);
                        fread(&find,sizeof(node_t),1,fp);
                    }

                    else if(prev.key > find.key)
                    {
                        int val;
                        val=find.right_offset;
                        fseek(fp,0,SEEK_SET);
                        fseek(fp,sizeof(tree_t)+sizeof(node_t)*val,SEEK_SET);
                        fread(&find,sizeof(node_t),1,fp);
                    }
            }
            prev.right_offset=val;
            fseek(fp,-sizeof(node_t),SEEK_CUR);
            fwrite(&prev,sizeof(node_t),1,fp);
        }
    }
    // if it has two nodes
    else
    {
        DELETE +=1;
        node_t rootnode;
        fseek(fp,0,SEEK_SET);
        fseek(fp,sizeof(tree_t)+sizeof(node_t)*head.root,SEEK_SET);
        fread(&rootnode,sizeof(node_t),1,fp);
        node_t success;
        node_t prevsuccess;
        fseek(fp,0,SEEK_SET);
        fseek(fp,sizeof(tree_t)+sizeof(node_t)*del.right_offset,SEEK_SET);
        fread(&success,sizeof(node_t),1,fp);
        if(success.left_offset == -1)
        {
            if(del.key == rootnode.key)
            {
                success.left_offset = del.left_offset;
                success.right_offset = del.right_offset;
                fseek(fp,0,SEEK_SET);
                fseek(fp,sizeof(tree_t)+sizeof(node_t)*del.right_offset,SEEK_SET);
                fwrite(&success,sizeof(node_t),1,fp);

                del.left_offset = head.free_head;
                del.right_offset = -1;
                fseek(fp,0,SEEK_SET);
                fseek(fp,sizeof(tree_t)+sizeof(node_t)*head.root,SEEK_SET);
                fwrite(&del,sizeof(node_t),1,fp);

                head.free_head = 0;
                head.root = rootnode.right_offset;
                fseek(fp,0,SEEK_SET);
                fwrite(&head,sizeof(tree_t),1,fp);
            }
            else if(del.key > prev.key)
            {
                prev.right_offset = del.right_offset;

                node_t find;
                fseek(fp,0,SEEK_SET);
                fseek(fp,sizeof(tree_t)+sizeof(node_t)*head.root,SEEK_SET);
                fread(&find,sizeof(node_t),1,fp);
                while(find.key != prev.key)
                {
                        if(prev.key < find.key)
                        {
                            int val;
                            val=find.left_offset;
                            fseek(fp,0,SEEK_SET);
                            fseek(fp,sizeof(tree_t)+sizeof(node_t)*val,SEEK_SET);
                            fread(&find,sizeof(node_t),1,fp);
                        }

                        else if(prev.key > find.key)
                        {
                            int val;
                            val=find.right_offset;
                            fseek(fp,0,SEEK_SET);
                            fseek(fp,sizeof(tree_t)+sizeof(node_t)*val,SEEK_SET);
                            fread(&find,sizeof(node_t),1,fp);
                        }
                }
                fseek(fp,-sizeof(node_t),SEEK_CUR);
                fwrite(&prev,sizeof(node_t),1,fp);

                success.left_offset = del.left_offset;

                fseek(fp,0,SEEK_SET);
                fseek(fp,sizeof(tree_t)+sizeof(node_t)*del.right_offset,SEEK_SET);
                fwrite(&success,sizeof(node_t),1,fp);

                del.left_offset = head.free_head;
                del.right_offset = -1;
                fseek(fp,0,SEEK_SET);
                fseek(fp,sizeof(tree_t)+sizeof(node_t)*delval,SEEK_SET);
                fwrite(&del,sizeof(node_t),1,fp);

                head.free_head = delval;
                fseek(fp,0,SEEK_SET);
                fwrite(&head,sizeof(tree_t),1,fp);
            }

            else if(del.key < prev.key)
            {
                prev.left_offset = del.right_offset;
                node_t find;
                fseek(fp,0,SEEK_SET);
                fseek(fp,sizeof(tree_t)+sizeof(node_t)*head.root,SEEK_SET);
                fread(&find,sizeof(node_t),1,fp);
                while(find.key != prev.key)
                {
                        if(prev.key < find.key)
                        {
                            int val;
                            val=find.left_offset;
                            fseek(fp,0,SEEK_SET);
                            fseek(fp,sizeof(tree_t)+sizeof(node_t)*val,SEEK_SET);
                            fread(&find,sizeof(node_t),1,fp);
                        }

                        else if(prev.key > find.key)
                        {
                            int val;
                            val=find.right_offset;
                            fseek(fp,0,SEEK_SET);
                            fseek(fp,sizeof(tree_t)+sizeof(node_t)*val,SEEK_SET);
                            fread(&find,sizeof(node_t),1,fp);
                        }
                }
                fseek(fp,-sizeof(node_t),SEEK_CUR);
                fwrite(&prev,sizeof(node_t),1,fp);

                success.left_offset = del.left_offset;
                fseek(fp,0,SEEK_SET);
                fseek(fp,sizeof(tree_t)+sizeof(node_t)*del.right_offset,SEEK_SET);
                fwrite(&success,sizeof(node_t),1,fp);

                del.left_offset = head.free_head;
                del.right_offset = -1;
                fseek(fp,0,SEEK_SET);
                fseek(fp,sizeof(tree_t)+sizeof(node_t)*delval,SEEK_SET);
                fwrite(&del,sizeof(node_t),1,fp);

                head.free_head = delval;
                fseek(fp,0,SEEK_SET);
                fwrite(&head,sizeof(tree_t),1,fp);
            }
        }
        else
        {
            
            while(success.left_offset != -1)
            {
                
                prevsuccess.left_offset = success.left_offset;
                prevsuccess.key = success.key;
                prevsuccess.right_offset = success.right_offset;
                fseek(fp,0,SEEK_SET);
                fseek(fp,sizeof(tree_t)+sizeof(node_t)*success.left_offset,SEEK_SET);
                fread(&success,sizeof(node_t),1,fp);
            }
            if(success.right_offset != -1)
            {
                node_t find;
                fseek(fp,0,SEEK_SET);
                fseek(fp,sizeof(tree_t)+sizeof(node_t)*head.root,SEEK_SET);
                fread(&find,sizeof(node_t),1,fp);
                while(find.key != prevsuccess.key)
                {
                        if(prevsuccess.key < find.key)
                        {
                            int val;
                            val=find.left_offset;
                            fseek(fp,0,SEEK_SET);
                            fseek(fp,sizeof(tree_t)+sizeof(node_t)*val,SEEK_SET);
                            fread(&find,sizeof(node_t),1,fp);
                        }

                        else if(prevsuccess.key > find.key)
                        {
                            int val;
                            val=find.right_offset;
                            fseek(fp,0,SEEK_SET);
                            fseek(fp,sizeof(tree_t)+sizeof(node_t)*val,SEEK_SET);
                            fread(&find,sizeof(node_t),1,fp);
                        }
                }
                prevsuccess.left_offset = success.right_offset;
                fseek(fp,-sizeof(node_t),SEEK_CUR);
                fwrite(&prevsuccess,sizeof(node_t),1,fp);
            }
            if(del.key == rootnode.key)
            {
                
                success.left_offset = del.left_offset;
                success.right_offset = del.right_offset;
                fseek(fp,0,SEEK_SET);
                fseek(fp,sizeof(tree_t)+sizeof(node_t)*prevsuccess.left_offset,SEEK_SET);
                fwrite(&success,sizeof(node_t),1,fp);

                node_t find;
                fseek(fp,0,SEEK_SET);
                fseek(fp,sizeof(tree_t)+sizeof(node_t)*head.root,SEEK_SET);
                fread(&find,sizeof(node_t),1,fp);
                while(find.key != prevsuccess.key)
                {
                        if(prevsuccess.key < find.key)
                        {
                            int val;
                            val=find.left_offset;
                            fseek(fp,0,SEEK_SET);
                            fseek(fp,sizeof(tree_t)+sizeof(node_t)*val,SEEK_SET);
                            fread(&find,sizeof(node_t),1,fp);
                        }

                        else if(prevsuccess.key > find.key)
                        {
                            int val;
                            val=find.right_offset;
                            fseek(fp,0,SEEK_SET);
                            fseek(fp,sizeof(tree_t)+sizeof(node_t)*val,SEEK_SET);
                            fread(&find,sizeof(node_t),1,fp);
                        }
                }
                prevsuccess.left_offset = -1;
                fseek(fp,-sizeof(node_t),SEEK_CUR);
                fwrite(&prevsuccess,sizeof(node_t),1,fp);

                del.left_offset = head.free_head;
                del.right_offset = -1;
                fseek(fp,0,SEEK_SET);
                fseek(fp,sizeof(tree_t)+sizeof(node_t)*head.root,SEEK_SET);
                fwrite(&del,sizeof(node_t),1,fp);

                head.free_head = 0;
                head.root = prevsuccess.left_offset;
                fseek(fp,0,SEEK_SET);
                fwrite(&head,sizeof(tree_t),1,fp);
            
            }
            else if(del.key > prev.key)
            {
                prev.right_offset = prevsuccess.left_offset;
                node_t find;
                fseek(fp,0,SEEK_SET);
                fseek(fp,sizeof(tree_t)+sizeof(node_t)*head.root,SEEK_SET);
                fread(&find,sizeof(node_t),1,fp);
                while(find.key != prev.key)
                {
                        if(prev.key < find.key)
                        {
                            int val;
                            val=find.left_offset;
                            fseek(fp,0,SEEK_SET);
                            fseek(fp,sizeof(tree_t)+sizeof(node_t)*val,SEEK_SET);
                            fread(&find,sizeof(node_t),1,fp);
                        }

                        else if(prev.key > find.key)
                        {
                            int val;
                            val=find.right_offset;
                            fseek(fp,0,SEEK_SET);
                            fseek(fp,sizeof(tree_t)+sizeof(node_t)*val,SEEK_SET);
                            fread(&find,sizeof(node_t),1,fp);
                        }
                }
                fseek(fp,-sizeof(node_t),SEEK_CUR);
                fwrite(&prev,sizeof(node_t),1,fp);
                prevsuccess.left_offset = -1;
                fseek(fp,0,SEEK_SET);
                fseek(fp,sizeof(tree_t)+sizeof(node_t)*head.root,SEEK_SET);
                fread(&find,sizeof(node_t),1,fp);
                while(find.key != prevsuccess.key)
                {
                        if(prevsuccess.key < find.key)
                        {
                            int val;
                            val=find.left_offset;
                            fseek(fp,0,SEEK_SET);
                            fseek(fp,sizeof(tree_t)+sizeof(node_t)*val,SEEK_SET);
                            fread(&find,sizeof(node_t),1,fp);
                        }

                        else if(prevsuccess.key > find.key)
                        {
                            int val;
                            val=find.right_offset;
                            fseek(fp,0,SEEK_SET);
                            fseek(fp,sizeof(tree_t)+sizeof(node_t)*val,SEEK_SET);
                            fread(&find,sizeof(node_t),1,fp);
                        }
                }
                fseek(fp,-sizeof(node_t),SEEK_CUR);
                fwrite(&prevsuccess,sizeof(node_t),1,fp);


                success.left_offset = del.left_offset;
                success.right_offset = del.right_offset;
                fseek(fp,0,SEEK_SET);
                fseek(fp,sizeof(tree_t)+sizeof(node_t)*prevsuccess.left_offset,SEEK_SET);
                fwrite(&success,sizeof(node_t),1,fp);
                del.left_offset = head.free_head;
                del.right_offset = -1;
                fseek(fp,0,SEEK_SET);
                fseek(fp,sizeof(tree_t)+sizeof(node_t)*delval,SEEK_SET);
                fwrite(&del,sizeof(node_t),1,fp);
                head.free_head = delval;
                fseek(fp,0,SEEK_SET);
                fwrite(&head,sizeof(tree_t),1,fp);
            }

            else if(del.key < prev.key)
            {
                prev.left_offset = prevsuccess.left_offset;
                node_t find;
                fseek(fp,0,SEEK_SET);
                fseek(fp,sizeof(tree_t)+sizeof(node_t)*head.root,SEEK_SET);
                fread(&find,sizeof(node_t),1,fp);
                while(find.key != prev.key)
                {
                        if(prev.key < find.key)
                        {
                            int val;
                            val=find.left_offset;
                            fseek(fp,0,SEEK_SET);
                            fseek(fp,sizeof(tree_t)+sizeof(node_t)*val,SEEK_SET);
                            fread(&find,sizeof(node_t),1,fp);
                        }

                        else if(prev.key > find.key)
                        {
                            int val;
                            val=find.right_offset;
                            fseek(fp,0,SEEK_SET);
                            fseek(fp,sizeof(tree_t)+sizeof(node_t)*val,SEEK_SET);
                            fread(&find,sizeof(node_t),1,fp);
                        }
                }
                fseek(fp,-sizeof(node_t),SEEK_CUR);
                fwrite(&prev,sizeof(node_t),1,fp);
                prevsuccess.left_offset = -1;
                fseek(fp,0,SEEK_SET);
                fseek(fp,sizeof(tree_t)+sizeof(node_t)*head.root,SEEK_SET);
                fread(&find,sizeof(node_t),1,fp);
                while(find.key != prevsuccess.key)
                {
                        if(prevsuccess.key < find.key)
                        {
                            int val;
                            val=find.left_offset;
                            fseek(fp,0,SEEK_SET);
                            fseek(fp,sizeof(tree_t)+sizeof(node_t)*val,SEEK_SET);
                            fread(&find,sizeof(node_t),1,fp);
                        }

                        else if(prevsuccess.key > find.key)
                        {
                            int val;
                            val=find.right_offset;
                            fseek(fp,0,SEEK_SET);
                            fseek(fp,sizeof(tree_t)+sizeof(node_t)*val,SEEK_SET);
                            fread(&find,sizeof(node_t),1,fp);
                        }
                }
                fseek(fp,-sizeof(node_t),SEEK_CUR);
                fwrite(&prevsuccess,sizeof(node_t),1,fp);

                success.left_offset = del.left_offset;
                success.right_offset = del.right_offset;
                fseek(fp,0,SEEK_SET);
                fseek(fp,sizeof(tree_t)+sizeof(node_t)*prevsuccess.left_offset,SEEK_SET);
                fwrite(&success,sizeof(node_t),1,fp);
                del.left_offset = head.free_head;
                del.right_offset = -1;
                fseek(fp,0,SEEK_SET);
                fseek(fp,sizeof(tree_t)+sizeof(node_t)*delval,SEEK_SET);
                fwrite(&del,sizeof(node_t),1,fp);
                head.free_head = delval;
                fseek(fp,0,SEEK_SET);
                fwrite(&head,sizeof(tree_t),1,fp);
            }
        }
    }
}

