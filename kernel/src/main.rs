#![no_std]
#![no_main]

mod vga_buffer;

use core::panic::PanicInfo;
use limine::BaseRevision;

// Request the Limine bootloader to provide a framebuffer//
// This is the first step towards graphics mode!
static FRAMEBUFFER_REQUEST: limine::request::FramebufferRequest = 
    limine::request::FramebufferRequest::new(BaseRevision::new(1, 0));

/// This is the main entry point for our kernel, called by Limine
#[no_mangle]
pub extern "C" fn _start() -> ! {
    // Ensure we have a framebuffer from the bootloader
    if let Some(framebuffer_response) = FRAMEBUFFER_REQUEST.get_response() {
        if let Some(framebuffer) = framebuffer_response.framebuffers().next() {
            // It worked! We have a graphics-mode framebuffer.
            // For now, let's just print to the screen.
            println!("Welcome to Sunkist OS!");
            println!("Booted with Limine.");

            // Let's draw something to prove we have graphics mode!
            // We'll fill the top-left 100x100 pixels with a solid color.
            let mut color: u32 = 0x00FF00FF; // A bright magenta
            let pitch = framebuffer.pitch() as usize;
            let width = framebuffer.width() as usize;
            let height = framebuffer.height() as usize;
            let base = framebuffer.addr();

            for y in 0..100 {
                for x in 0..100 {
                    unsafe {
                        // Calculate the pixel offset and write the color
                        let offset = y * pitch + x * 4;
                        core::ptr::write_volatile(base.add(offset) as *mut u32, color);
                    }
                }
            }
        }
    } else {
        // This will run if we fail to get a framebuffer
        println!("Failed to get framebuffer!");
    }


    loop {}
}

#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
    println!("{}", info);
    loop {}
}
