
// Put in C .h file: extern void rust_function();
#[no_mangle]
pub extern "C" fn rust_function() {
    println!("********** Hello World!");
}
