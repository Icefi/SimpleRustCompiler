fn main() {
    let array: [i32; 10] = [10, 3, 5, 8, 11, 34, 6, 12, 9, 19];

    let i = 0;
    let min = array[0];
    let tmp = 0;

    while i < 10 {
        scan("%d", tmp);
        array[i] = tmp;
        i = i + 1;
    }

    i = 0;
    while i < 10 {
        if array[i] < min {
            min = array[i];
        }

        i = i + 1;
    }


    print("\nmin is %d\n", min);
}
