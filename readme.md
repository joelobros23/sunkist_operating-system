# **Sunkist OS**

Sunkist OS is an experimental, 64-bit operating system designed from the ground up to be lightweight, secure, and controlled by a powerful AI core. It blends the safety and robustness of Rust for the kernel with the high performance of C++ for a modern, fluid graphical user interface.

## **Core Philosophy**

* **AI-Native:** The primary user interface is not a traditional desktop but a conversational, context-aware AI. Sunkist OS is designed to be operated through natural language commands, powered by models like Gemini.  
* **Secure & Robust Kernel:** The entire kernel, including drivers and system services, is written in Rust to leverage its compile-time memory and thread safety guarantees, eliminating many common OS-level vulnerabilities.  
* **High-Performance GUI:** The desktop shell, window manager, and compositor will be built in C++ for maximum performance and direct access to graphics hardware, ensuring a responsive and visually rich experience.  
* **Futuristic & Minimalist:** The user interface aesthetic is clean, modern, and uncluttered, using Lucide for iconography and emphasizing smooth animations and a consistent design language.

## **Current Status: Phase 0 Complete**

The project has successfully completed its initial milestone:

* **A bootable 64-bit kernel** written in Rust.  
* The kernel uses its own custom entry point and does not rely on the standard library (no\_std).  
* It can successfully print text to the VGA buffer, displaying a welcome message on boot.  
* The project builds and runs in the QEMU emulator.

## **High-Level Architecture**

\+------------------------------------------------------+  
|         User Applications (Future Scope)             |  
\+------------------------------------------------------+  
|        GUI / Desktop Shell / Window Manager (C++)      |  
\+------------------------------------------------------+  
|                   AI Core (Rust)                     |  
\+------------------------------------------------------+  
|           System Services & Daemons (Rust)           |  
\+------------------------------------------------------+  
|                FFI Bridge (Rust/C++)                 |  
\+------------------------------------------------------+  
|                Sunkist Kernel (Rust)                 |  
\+------------------------------------------------------+  
|              Hardware Abstraction Layer (HAL)        |  
\+------------------------------------------------------+  
|                       Hardware                       |  
\+------------------------------------------------------+

## **How to Build and Run.**

1. **Prerequisites:**  
   * Rust (nightly toolchain)  
   * rustup component add llvm-tools-preview  
   * cargo install bootimage  
   * QEMU emulator  
2. **Navigate to the kernel directory:**  
   cd sunkist\_os/kernel

3. **Run the OS:**  
   cargo run

   This command will compile the kernel, create a bootable disk image, and launch it in QEMU.

## **Next Steps**

With the basic kernel booting, the next major phases are:

* **Phase 1: Kernel Foundations:** Implement fundamental OS features like interrupt handling (for keyboard/mouse input), memory management, and basic process scheduling.  
* **Phase 2: The Rust/C++ Bridge & Graphics:** Establish the connection between the Rust kernel and a C++ graphics library, with the goal of drawing the first pixels and initializing a graphical display mode.