package main
import "fmt"
import "os"

func main() {
	file, err := os.OpenFile("message.txt", os.O_RDWR|os.O_CREATE, 0666)
	if err != nil {
        panic(err)
	}
	defer file.Close()
	_, err = file.WriteString("Do you have what it takes to be an engineer at TheStartup™?\nAre you willing to work 80 hours a week in hopes that your 0.001% equity is worth something?\nCan you say \"synergy\" and \"democratize\" with a straight face?\nAre you prepared to eat top ramen at your desk 3 meals a day?\nend")
	if err != nil {
		panic(err)
	}
	fmt.Println("I hope I get the job!")
}
