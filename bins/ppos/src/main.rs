#![no_std]
#![no_main]

extern crate alloc;

mod driver;
mod memory;
mod shell;

use core::{arch::global_asm, panic::PanicInfo};
use library::println;
use shell::Shell;

global_asm!(include_str!("boot.s"));

#[no_mangle]
pub unsafe fn _start_rust() -> ! {
    kernel_start();
}

unsafe fn kernel_start() -> ! {
    unsafe {
        driver::init().unwrap();
    }
    let mut shell = Shell::new();
    shell.run();
}

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    println!("{}", _info);
    loop {}
}