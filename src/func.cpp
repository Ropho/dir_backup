#include "../include/header.hpp"

char logger[1024];


void backup (const char *src_parent_name, DIR *src,
             const char *dst_parent_name, DIR *dst) {

    while (1) {

        errno = 0;
        struct dirent *src_tmp = readdir (src);
        if (src_tmp == NULL) {
            
            if (errno != 0) {
                ERROR ("READDIR ERROR");
            }
            else {
                return;
            }
        }
        else {

            char src_path[1024];
            snprintf (src_path, sizeof (src_path), "%s/%s", src_parent_name, src_tmp->d_name);

            char dst_path[1024];
            snprintf (dst_path, sizeof (dst_path), "%s/%s", dst_parent_name, src_tmp->d_name);

            switch (src_tmp -> d_type) {

                case DT_DIR: {
                    
                    strcpy (logger, "\nDIR: ");
                    LOG (strcat (logger, src_tmp->d_name));
                    // printf ("DIR: %s\n", src_tmp->d_name);

                    if (strcmp (src_tmp->d_name, ".") == 0 || strcmp (src_tmp->d_name, "..") == 0)
                        continue;

                    // printf ("WANT OPEN DIR: %s\n", path);

                    rewinddir (dst);

                    while (1) {
                    
                        errno = 0;
                        struct dirent *dst_tmp = readdir (dst);


                        struct stat src_stat {};
                        if (stat (src_path, &src_stat) < 0) {
                            ERROR ("STAT SRC FILE");
                        }
                        int mode = src_stat.st_mode & ((1 << 10) - 1);

                        if (dst_tmp == NULL) {
                            
                            if (errno != 0) {
                                ERROR ("READDIR DST ERROR");
                            }

                            else {
                                NOTE ("\tNO DIR FOUND, CREATING NEW");
                                    // printf ("MODE: %o\n", mode);
                                    if (mkdir (dst_path, mode) < 0) {
                                        ERROR ("NEW DIR CREAT ERROR");
                                    }

                                break;
                            }
                        }
                        else {
                            if (strcmp (src_tmp->d_name, dst_tmp->d_name) == 0) {
                                NOTE ("\tFILE FOUND");

                                struct stat dst_stat {};
                                if (stat (dst_path, &dst_stat) < 0) {
                                    ERROR ("STAT SRC FILE");
                                }
                                if (chmod (dst_path, src_stat.st_mode) < 0) {
                                    ERROR ("CHMOD DST ERROR");
                                }

                                break;
                            }
                        }
                    }


                    DIR *tmp_src_dir = opendir (src_path);
                    if  (tmp_src_dir == NULL) {
                        ERROR ("TMP SRC DIR OPEN ERROR");
                    }

                    DIR *tmp_dst_dir = opendir (dst_path);
                    if  (tmp_dst_dir == NULL) {
                        ERROR ("TMP DST DIR OPEN ERROR");
                    }

                    backup (src_path, tmp_src_dir,
                            dst_path, tmp_dst_dir);
                
                break;}

                case DT_REG: {

                    strcpy (logger, "\nREGULAR: ");
                    LOG (strcat (logger, src_tmp->d_name));

                    rewinddir (dst);

                    while (1) {
                    
                        errno = 0;
                        struct dirent *dst_tmp = readdir (dst);


                        struct stat src_stat {};
                        if (stat (src_path, &src_stat) < 0) {
                            ERROR ("STAT SRC FILE");
                        }
                        int mode = src_stat.st_mode & ((1 << 10) - 1);

                        if (dst_tmp == NULL) {
                            
                            if (errno != 0) {
                                ERROR ("READDIR DST ERROR");
                            }

                            else {
                                NOTE ("\tNO FILE FOUND, CREATING NEW");
                                    // printf ("MODE: %o\n", mode);
                                    if (creat (dst_path, mode) < 0) {
                                        ERROR ("NEW FILE CREAT ERROR");
                                    }

                                    int fd1 = open (src_path, O_RDWR);
                                    if (fd1 < 0) {
                                        ERROR ("OPEN SRC FILE ERROR");
                                    }
                                    int fd2 = open (dst_path, O_RDWR);
                                    if (fd2 < 0) {
                                        ERROR ("OPEN DST FILE ERROR");
                                    }
                                    char buf[1024];
                                    // read (fd1, )
                                    int size = 0;
                                    while (1) {
                                        size = read (fd1, buf, sizeof (buf));
                                        if (size == 0) {
                                            break;
                                        }
                                        else if (size < 0) {
                                            ERROR ("READ FROM SRC ERROR");
                                        }
                                        if (write (fd2, buf, size) != size) {
                                            ERROR ("WRITE TO DST ERROR");
                                        }
                                    }
                                    if (close (fd1) < 0) {
                                        ERROR ("CLOSE SRC FILE ERROR");
                                    }
                                    if (close (fd2) < 0) {
                                        ERROR ("CLOSE DST FILE ERROR");
                                    }
                                    
                                break;
                            }
                        }
                        else {
                            if (strcmp (src_tmp->d_name, dst_tmp->d_name) == 0) {
                                NOTE ("\tFILE FOUND");

                                struct stat dst_stat {};
                                if (stat (dst_path, &dst_stat) < 0) {
                                    ERROR ("STAT SRC FILE");
                                }
                                if (chmod (dst_path, src_stat.st_mode) < 0) {
                                    ERROR ("CHMOD DST ERROR");
                                }

                                // printf ("TIME SRC: %ld\nTIME DST: %ld\n", src_stat.st_mtime, dst_stat.st_mtime);
                                if (src_stat.st_mtime > dst_stat.st_mtime) {
                                    
                                    NOTE("\tMODIFYING");

                                    int fd1 = open (src_path, O_RDWR);
                                    if (fd1 < 0) {
                                        ERROR ("OPEN SRC FILE ERROR");
                                    }
                                    int fd2 = open (dst_path, O_RDWR);
                                    if (fd2 < 0) {
                                        ERROR ("OPEN DST FILE ERROR");
                                    }
                                    char buf[1024];
                                    // read (fd1, )
                                    int size = 0;
                                    while (1) {
                                        size = read (fd1, buf, sizeof (buf));
                                        if (size == 0) {
                                            break;
                                        }
                                        else if (size < 0) {
                                            ERROR ("READ FROM SRC ERROR");
                                        }
                                        if (write (fd2, buf, size) != size) {
                                            ERROR ("WRITE TO DST ERROR");
                                        }
                                    }

                                    if (close (fd1) < 0) {
                                        ERROR ("CLOSE SRC FILE ERROR");
                                    }
                                    if (close (fd2) < 0) {
                                        ERROR ("CLOSE DST FILE ERROR");
                                    }

                                }

                                break;
                            }
                        }
                    }

                break;}

                default: {
                    // printf ("%d\n", src_tmp->d_type);
                    ERROR ("UNKNOWN TYPE OF FILE (NOT DIR / REGULAR)");
                break;}
            }
        }

    }
}