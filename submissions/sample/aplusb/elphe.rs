fn main() {
    let stdin = std::io::read_to_string(std::io::stdin()).unwrap();
    let mut stdin = stdin.split_ascii_whitespace();

    let a: u32 = stdin.next().unwrap().parse().unwrap();
    let b: u32 = stdin.next().unwrap().parse().unwrap();
    println!("{}", a + b);
}
