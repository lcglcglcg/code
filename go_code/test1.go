
package main
import (
	"http";
	"io";
	"runtime";
       )

func HelloServer(c *http.Conn, req *http.Request) {
    io.WriteString(c, "hello, world!\n");
}


func main() {
    runtime.GOMAXPROCS(1); // 1 cores
    http.Handle("/", http.HandlerFunc(HelloServer));
	err := http.ListenAndServe(":8000",nil);
     if err != nil {
	 panic("ListenAndServe: ", err.String());
     }
} 
