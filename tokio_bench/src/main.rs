#[tokio::main]
async fn main() {
    let args: Vec<String> = std::env::args().collect();
    if args.len() < 2 {
        print!("Please specify the number of tasks\nExample: {} 1000\n", args[0]);
        return;
    }
    let mut tasks = Vec::new();
    for _i in 0..args[1].parse().unwrap() {
        tasks.push(tokio::spawn(async { tokio::time::sleep(std::time::Duration::from_secs(10)).await }));
    }
    for task in tasks {
        task.await.unwrap();
    }
}
