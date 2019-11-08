char* itoa10buffer(unsigned int value, char* c_buffer){
        // c_buffer needs to be size 10
        static char presentation[] = "0123456789";
        int i = 8; // start from the back
        do { // need do while in case value = 0
                c_buffer[i] = presentation[value % 10];
                --i;
                value /= 10;
        } while (value > 0);
        return c_buffer;
}

char* itoa16buffer(unsigned int value, char* c_buffer){

        //c_buffer needs to be size 8
        static char presentation[] = "0123456789abcdef";
        int i = 6; // start from the back
        do { // need do while in case value = 0
                c_buffer[i] = presentation[value % 16];
                --i;
                value /= 16;
        } while (value > 0);
        return c_buffer;
}

void setEmptyString(char c_buffer[], int buf_len){
        for(int i=0; i < buf_len; i++) {
                c_buffer[i] = '\0';
        }
}

char* itoa10buffer(unsigned int value, char* c_buffer){
        // c_buffer needs to be size 10
        static char presentation[] = "0123456789";
        int i = 8; // start from the back
        do { // need do while in case value = 0
                c_buffer[i] = presentation[value % 10];
                --i;
                value /= 10;
        } while (value > 0);
        return c_buffer;
}

char* itoa16buffer(unsigned int value, char* c_buffer){

        //c_buffer needs to be size 8
        static char presentation[] = "0123456789abcdef";
        int i = 6; // start from the back
        do { // need do while in case value = 0
                c_buffer[i] = presentation[value % 16];
                --i;
                value /= 16;
        } while (value > 0);
        return c_buffer;
}