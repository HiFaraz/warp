package main
import "os"
import "net/http"

func helloWorld(w http.ResponseWriter, r *http.Request) {
	w.Write([]byte("Hello World\n"))
}
func main() {
	http.HandleFunc("/", helloWorld)
	http.ListenAndServe(":" + os.Getenv("PORT"), nil)
}