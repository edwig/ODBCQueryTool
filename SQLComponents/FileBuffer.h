#ifndef __FILEBUFFER__
#define __FILEBUFFER__

// Max streaming serialize/deserialize limit
// Files bigger than this can only be putted/gotten by indirect file references
// This is also the streaming limit for OData / REST interfaces
#define STREAMING_LIMIT    (100*1024*1024)  // 100 Megabyte

// Static globals for the server as a whole
// Can be set through the web.config reading of the HTTPServer
extern unsigned long g_streaming_limit; // = STREAMING_LIMIT;

#endif
