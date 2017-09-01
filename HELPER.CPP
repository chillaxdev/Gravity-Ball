/*
--------------------------------
 #####   #     #  ###  #     #
 #       #     #   #   #     #
 #       #     #   #   #     #
 #####   #######   #   #     #
     #   #     #   #   #     #
     #   #     #   #    #   #
 #####   #     #  ###     #
--------------------------------
 ///      HELPER FILE       ///
--------------------------------
 */

#include <iostream.h>
#include <string.h>
#include <graphics.h>
#include <process.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#include <fstream.h>

#define MAX_RECORDS 1024

class CompatibleString {
char data[512];

public:
CompatibleString() {
        data[0] = '\0';
}

CompatibleString(char str[512]) {
        strcpy(data, str);
}

void setData(char str[512]) {
        strcpy(data, str);
}

CompatibleString append(CompatibleString str) {
        strcat(data, str.getData());
        return *this;
}


int equals(char *data) {
        return strcmp(this->data, data);
}

int equalsIgnoreCase(char *data) {
        return strcmpi(this->data, data);
}


void setValueAtSubscript(int pos, char value) {
        data[pos] = value;
}

CompatibleString(CompatibleString &compatibleString) {
        strcpy(data, compatibleString.data);
}

char *getData() {
        return data;
}

char getChar(int pos) {
        return data[pos];
}

};

typedef CompatibleString String;

class Mouse {
REGS in, out;
SREGS sys;

public:
Mouse() {
        in.x.ax = 1;
        int86(0x33, &in, &out);
}

void show() {
        in.x.ax = 1;
        int86(0x33, &in, &out);
}

void getPos(int &x, int &y, int &clicked) {
        in.x.ax = 3;
        int86(51, &in, &out);
        clicked = out.x.bx;
        x = out.x.cx;
        y = out.x.dx;
}

void hide() {
        in.x.ax = 2;
        int86(51, &in, &out);
}

};


//constantinos
struct _Folder {
        String USERS;
        String LOGS;
        String LEVEL_START;

        _Folder() {
                USERS = "USERS.DAT";
                LOGS = "LOGS.DAT";
                //LEVELS = "\\levels\\START.DAT";
                LEVEL_START = "C:\\TC\\BIN\\LEVELS\\START";
        }

} Folder;

struct _Dat {
        String TERRAIN;
        String COLLECTIBLE;
        String POSITIVE_POINT;
        String MEGA_POSITIVE_POINT;
        String NEGATIVE_POINT;
        String MEGA_NEGATIVE_POINT;
        String TELEPORT;
        String VERTICAL_TERRAIN;


        _Dat() {
                COLLECTIBLE = "COLLECTIBLE.DAT";
                TERRAIN = "TERRAIN.DAT";
                POSITIVE_POINT = "POSITIVE_POINT.DAT";
                MEGA_POSITIVE_POINT = "MEGA_POSITIVE_POINT.DAT";
                NEGATIVE_POINT = "NEGATIVE_POINT.DAT";
                MEGA_NEGATIVE_POINT = "MEGA_NEGATIVE_POINT.DAT";
                TELEPORT = "TELEPORT.DAT";
                VERTICAL_TERRAIN = "VTERRAIN.DAT";
        }

} Dat;

struct _Key {
        char BACKSPACE;
        char UP;
        char ESCAPE;
        char ENTER;
        char INVALID;
        char TAB;

        _Key() {
                BACKSPACE = 8;
                UP = 72;
                ESCAPE = 27;
                INVALID = -1;
                ENTER = 13;
                TAB = 9;
        }

} Key;

struct _Graphics {
        int DRIVER;
        int MODE;

        //Multi page supported driver and corresponding mode
        _Graphics() {
                DRIVER = VGA;
                MODE = VGAMED;
        }
} Graphics;

struct _File {
        int BINARY;
        int TEXT;

        _File() {
                BINARY = 1;
                TEXT = 0;
        }

} Files;

class GraphicsEngine {
int page;

public:
GraphicsEngine() {
        page = 0;
}
void setScreen() {
        if (page == 0) {
                setactivepage(1);
        }
        else if (page == 1) {
                setactivepage(0);
        }
}

void showScreen() {
        if (page == 0) {
                page = 1;
                setvisualpage(page);
        }
        else if (page == 1) {
                page = 0;
                setvisualpage(page);
        }
}
};

class File {
fstream fstreamInstance;

public:

void open(String uri, int mode) {
        if(mode == Files.BINARY) {
                fstreamInstance.open(uri.getData(), ios::in | ios::out | ios::binary);
        } else {
                fstreamInstance.open(uri.getData(), ios::in | ios::out);
        }
}

istream &read(char *var, int size) {
        return fstreamInstance.read(var, size);
}

ostream &write(char *var, int size) {
        return fstreamInstance.write(var, size);
}

int getStatus() {
        if(!fstreamInstance) {
                return 0;
        }
        return 1;
}

void moveToEnd() {
        fstreamInstance.seekp(0, ios::end);
}

void moveToPreviousRecord(int size) {
        fstreamInstance.seekp(-size, ios::cur);
}

void close() {
        fstreamInstance.close();
}

};
