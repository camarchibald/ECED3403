// Created by Cameron Archibald B00893056 2024-17-05.
// entries.c - handles reading from file, parsing the S records to get data and other info from hex chars to numbers
// and determining validity

#include "entries.h"

//#define mov //Test 1
//#define arithmetic //Test 2
//#define shift //Test 6

//Read each line and load as applicable
void readentry(FILE *infile) {
    entry record;
    //Initially true to enter the while loop
    record.checksum = true;
    record.recordtype = 0;

    //Facilitate testing by preloading registers
#ifdef mov
    regfile[REG][R0].word = 0x0C0D;
    regfile[REG][R1].word = 0x1A01;
    regfile[REG][R2].word = 0x0C0D;
    regfile[REG][R3].word = 0x0C0D;
    regfile[REG][R4].word = 0x0C0D;
    regfile[REG][R5].word = 0x0C0D;
    regfile[REG][R6].word = 0x0C0D;
#endif

#ifdef arithmetic
    regfile[REG][R0].word = 0x0F02;
    regfile[REG][R1].word = 0x010F;
    regfile[REG][R2].word = 0x0004;
    regfile[REG][R3].word = 0x0001;
    regfile[REG][R4].word = 0x1A89;
    regfile[REG][R5].word = 0x1F12;
#endif

#ifdef shift
    regfile[REG][R0].word = 0x8FF1;
    regfile[REG][R1].word = 0x0121;
    regfile[REG][R2].word = 0x0001;
    regfile[REG][R3].word = 0x0A00;
#endif


    //While lines left to read and the checksum is true and recordtype is not INVALID
    while (fgets(record.string, MAXRECLEN, infile) && record.checksum && (record.recordtype != INVALID)) {
        //Parse the line and fill in the entry struct
        record = parseentry(record);

        //If checksum true, load, else report error
        if (record.recordtype == INVALID)
            printf("Entry %s is invalid\n", record.string);
        else if (record.checksum)
            loader(record);
        else
            printf("Entry %s has an invalid checksum\n", record.string);
    }
}

//Parse the string to fill out the entry struct
entry parseentry(entry rec) {
    //Initialize checksum to 0
    char sum = 0;

    //Get record type, pass string[1] to assignrecordtype
    rec.recordtype = assignrecordtype(rec.string[RECTYPESTART]);

    //Length byte (addr + data)
    rec.len = hextobyte(rec.string[LENSTART], rec.string[LENSTART + 1]);
    //Add byte to checksum
    sum += (char) rec.len;

    //Addr word, get low and high bytes from string at indices 4,5,6,7
    unsigned char addrhigh = hextobyte(rec.string[ADDRSTART], rec.string[ADDRSTART + 1]);
    unsigned char addrlow = hextobyte(rec.string[ADDRSTART + 2], rec.string[ADDRSTART + 3]);
    //Address = MSByte << 8 + LSbyte
    rec.address = addrhigh * (BYTESIZE) + addrlow;
    //Add bytes to checksum
    sum += (char) addrhigh + (char) addrlow;

    //Pointer to the start of the data
    char *ptr = &rec.string[DATASTART];

    //Cycle through data, convert nibble pairs to unsigned char and place in hexdata
    //Read only data, not address or checksum
    for (int i = 0; i < rec.len - (ADDRBYTES + CHECKBYTES); i++) {
        //Read nibble pair (current char and next char)
        rec.hexdata[i] = hextobyte(*ptr, *(ptr + sizeof(*ptr)));
        //Add byte to checksum
        sum += (char) rec.hexdata[i];
        //Move to next nibble pair
        ptr++;
        ptr++;
    }

    //Compute value of final nibble pair
    char checkval = hextobyte(*ptr, *(ptr + sizeof(*ptr)));
    //If running sum + checksum == -1, line is valid
    if ((sum + checkval) == -1)
        rec.checksum = true;
    else
        rec.checksum = false;

    //Check if record is still valid based on parsed data
    rec = recordvalid(rec);

    return rec;
}

//Convert the char from the S record (S0,1,2,9) into the correct enum (recordtype)
int assignrecordtype(char sx) {
    int recordtype;
    switch (sx) {
        case '0':
            recordtype = S0;
            break;
        case '1':
            recordtype = S1;
            break;
        case '2':
            recordtype = S2;
            break;
        case '9':
            recordtype = S9;
            break;
        default:
            recordtype = INVALID;
            break;
    }
    return recordtype;
}

//Returns the decimal value of two nibbles
unsigned char hextobyte(char msnibble, char lsnibble) {
    //Most significant nibble left shifted by 4 | least significant nibble
    return hextoint(msnibble) * (NIBBLESIZE) + hextoint(lsnibble);
}

//Returns the decimal value (0-15) of hex (0-9,A-F)
unsigned char hextoint(char c) {
    switch (c) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            c = c - '0';
            break;
        case 'A':
            c = 10;
            break;
        case 'B':
            c = 11;
            break;
        case 'C':
            c = 12;
            break;
        case 'D':
            c = 13;
            break;
        case 'E':
            c = 14;
            break;
        case 'F':
            c = 15;
            break;
    }
    return c;
}

//Check for out of bounds addr/length, make recordtype INVALID so it isn't loaded
entry recordvalid(entry rec) {
    if ((rec.address > (DATASIZE)) || //Starting address out of bounds
        (rec.address < 0) || //Address negative
        (rec.len < ADDRBYTES + CHECKBYTES) || //Length less than address bytes + check bytes (implies a negative number of data bytes)
        (rec.address + rec.len > (DATASIZE))) //Addr + len is out of bounds (meaning the last byte to copy is out of bounds)
    {
        //Update type to invalid
        rec.recordtype = INVALID;
    }
    return rec;
}
