#include <acpi/acpi.hpp>
#include <dDraw/pixels.hpp>
#include <debug.hpp>
#include <dev/framebuffer.hpp>
#include <exe/elf/elf.hpp>
#include <hal/apic/apic.hpp>
#include <hal/hal.hpp>
#include <hal/io.hpp>
#include <hal/ioapic/ioapic.hpp>
#include <hal/pcie/pcie.hpp>
#include <mem/heap.hpp>
#include <mem/malloc.h>
#include <mem/physical_memory.hpp>
#include <mem/virtual_memory.hpp>
#include <nlohmann/json.hpp>
#include <smbios/smbios.hpp>
#include <stdint.h>
#include <string.h>
#include <sys/modules.hpp>
#include <sys/runtime.hpp>
#include <sys/symbols.hpp>
#include <sys/test/tests.hpp>
#include <sys/types.h>
#include <us/syscalls.hpp>

using json = nlohmann::json;

limine_framebuffer *main_framebuffer = nullptr;

void timer()
{
    debug_printf(".");
}

void set_pit_frequency(uint16_t frequency)
{
    uint16_t divisor = 1193180 / frequency;

    // Send control word to PIT (Channel 0, Mode 2, Binary)
    outb(0x43, 0x34);

    // Send the low byte of the divisor to Channel 0
    outb(0x40, (uint8_t)(divisor & 0xFF));

    // Send the high byte of the divisor to Channel 0
    outb(0x40, (uint8_t)(divisor >> 8));
}

extern "C" void kernel_main()
{
    initialize_runtime();
    initialize_framebuffer();
    initialize_flanterm();
    initialize_hal();
    initialize_phys_memory();
    initialize_virtual_memory();
    initialize_heap();
    initialize_acpi();
    initialize_smbios();
    initialize_pcie();
    initialize_apic();
    initialize_ioapic();
    initialize_irqs();
    // initialize_syscall(reinterpret_cast<uint64_t>(&syscall_handler), 0x8, 0x10, 0x8, 0x10, ~(1 << 9));

    // Initialize framebuffer after memory management
    main_framebuffer = get_framebuffer(0); // Get first framebuffer.
    if (main_framebuffer == nullptr)
    {
        debug_printf("No framebuffer has been found.\n");
        asm volatile("cli; hlt"); // failed to get framebuffer.
    }

    auto statistics = get_memory_statistics();
    debug_printf("Overall: %lu (MB) Usable: %lu (KB)\n", statistics->overall / 1024 / 1024, statistics->usable / 1024);

    print_modules();

    const char *module_path = "/template.kmod";
    auto test_kernel_module_file = get_module(module_path);
    if (test_kernel_module_file.address == nullptr)
    {
        debug_printf("No kernel module found (%s).\n", module_path);
        asm volatile("cli; hlt");
    }

    auto bgrt_width = get_bgrt_bitmap_width();
    auto bgrt_height = get_bgrt_bitmap_height();

    if (bgrt_width != -1 && bgrt_height != -1)
    {
        auto fb = get_framebuffer(0);
        draw_bgrt_bitmap(fb, fb->width - bgrt_width, fb->height - bgrt_height);
    }

    // do_tests();

    // print_smbios_entries();

    // attach_irq_handler(0, timer);
    // set_pit_frequency(2);
    // print_ioapic_descriptors();

    // asm volatile ("syscall");
    // debug_printf("test");

    // JSON string

    // Example JSON string
    std::string json_str = R"({
        "name": "John",
        "age": 30,
        "isStudent": false,
        "courses": ["Math", "Science"]
    })";

    // Parse the JSON string
    json j = json::parse(json_str);

    // Accessing individual values
    std::string name = j["name"];
    int age = j["age"];
    bool isStudent = j["isStudent"];
    std::vector<std::string> courses = j["courses"];

    // Output the parsed data using debug_printf
    debug_printf("Name: %s\n", name.c_str());
    debug_printf("Age: %d\n", age);
    debug_printf("Is Student: %s\n", isStudent ? "Yes" : "No");
    debug_printf("Courses: ");
    for (const auto &course : courses)
    {
        debug_printf("%s ", course.c_str());
    }
    debug_printf("\n");

    // Modify the JSON object
    j["age"] = 31;         // Update the age
    j["isStudent"] = true; // Update isStudent status

    // Add a new key-value pair
    j["email"] = "john.doe@example.com";

    // Serialize the modified JSON object back to a string
    std::string updated_json = j.dump(4); // The "4" makes the output pretty-printed with 4 spaces of indentation

    debug_printf("Updated JSON:\n%s\n", updated_json.c_str());

    while (true)
    {
        asm volatile("nop");
    }
}
