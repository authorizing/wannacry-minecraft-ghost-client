
#include "scanner.h"

typedef enum _MEMORY_INFORMATION_CLASS {
    MemoryBasicInformation
} MEMORY_INFORMATION_CLASS;

extern "C" NTSTATUS ZwQueryVirtualMemory(HANDLE, void*, MEMORY_INFORMATION_CLASS, void*, SIZE_T, SIZE_T*);

namespace wcry {

    std::once_flag calculator_flag;

    namespace scanner {

        size_t pattern(size_t min, size_t max, std::vector<int> pattern) {

            size_t pattern_size = pattern.size();

            for (size_t address = min; address < max; address += 100000) {

                std::vector<byte>memory(100000);
                if (!NT_SUCCESS(ZwReadVirtualMemory(wcry::p_handle, (LPVOID)address, &memory[0], 100000, 0)))
                    continue;

                for (size_t it = 0; it < 100000; ++it) {

                    if (address + it >= max) break;

                    bool should_check = true;

                    if (pattern[0] != RAND) {

                        if (memory[it] != pattern[0])
                            should_check = false;

                    }

                    if (should_check) {

                        bool is_pattern = true;

                        for (size_t x = 0; x < pattern.size(); ++x) {

                            if (pattern[x] == RAND) continue;

                            if (pattern[x] != memory[it + x]) {

                                is_pattern = false;
                                break;

                            }

                        }

                        if (is_pattern) return address + it;

                    }

                }

            }

            return 0;

        }

        //redefinitions so compiler doesnt freak out
        //i hate namespaces

        //reach
        std::vector<size_t> addresses::reach::doubles;
        std::vector<size_t> addresses::reach::unlimit;
        std::vector<size_t> addresses::reach::floats;

        //glide
        std::vector<size_t> addresses::glide::locationsglide;

        //nametags
        std::vector<size_t> addresses::nametags::locations1;
        std::vector<size_t> addresses::nametags::locations2;

        //esp
        std::vector<size_t> addresses::esp::locations;

        //cavefinder
        std::vector<size_t> addresses::cavefinder::locations;

        //timer
        std::vector<size_t> addresses::timer::locations;

        //bhop
        std::vector<size_t> addresses::bhop::locations;

        //speed
        std::vector<size_t> addresses::speed::locations;


        template<class val>
        std::vector<size_t> get_positions(std::vector<val> buffer, val value, size_t times_found) {
            std::vector<size_t> positions;
            size_t pos = std::distance(buffer.begin(), find(buffer.begin(), buffer.end(), value));
            size_t lastpos = pos;

            while (pos < buffer.size()) {
                if (positions.size() >= times_found)
                    break;
                positions.push_back(pos);
                size_t calc = pos + 1;
                pos = std::distance(buffer.begin(), find(buffer.begin() + calc, buffer.end(), value));
                if (pos == lastpos)
                    break;
            }
            return positions;
        }

        size_t min_address = 0x02000000;
        size_t max_address = 0x08000000;

        void calculator() {

            while (true) {
                bool found = false;

                size_t address = 0x2000000;

                MEMORY_BASIC_INFORMATION mbi;

                while (NT_SUCCESS(ZwQueryVirtualMemory(wcry::p_handle, (void*)address, MemoryBasicInformation,
                    &mbi, sizeof(mbi), nullptr))) {
                    if (mbi.State == MEM_COMMIT && ((mbi.Protect == PAGE_EXECUTE_READWRITE) | (mbi.Protect == PAGE_READWRITE) |
                        (mbi.Protect == PAGE_EXECUTE_WRITECOPY) |
                        (mbi.Protect == PAGE_WRITECOPY))) {

                        if (!found) {
                            min_address = address;
                            found = !found;
                        }

                    }

                    if (address + mbi.RegionSize >= 0x10000000)
                        break;
                    else
                        address += mbi.RegionSize;
                }
                max_address = address;

                Sleep(1000 * 10);

            }

        }

        void get_values() {

            std::call_once(calculator_flag, []() {

                CreateThread(nullptr, 0,
                    (LPTHREAD_START_ROUTINE)&calculator,
                    nullptr, 0, nullptr);

                });

            DWORD status = 0;
            if (GetExitCodeProcess(wcry::p_handle, &status)) {
                if (status != STILL_ACTIVE)
                    exit(0);
            }

            for (size_t address = min_address; address <= max_address; address += 4096) {
                std::vector<double> memoryd(4096);
                std::vector<float> memoryf(4096);

                bool doubleread = NT_SUCCESS(ZwReadVirtualMemory(wcry::p_handle, (void*)address, &memoryd[0], 4096,
                    nullptr));
                bool floatread = false;

                if (doubleread)
                    floatread = NT_SUCCESS(ZwReadVirtualMemory(wcry::p_handle, (void*)address, &memoryf[0], 4096, nullptr));

                if (doubleread && floatread) {
                    if (size_t x = std::count(memoryd.begin(), memoryd.end(), 4.5)) {
                        for (const size_t location : get_positions(memoryd, 4.5, x)) {
                            addresses::reach::unlimit.push_back(address + location * sizeof(double));

                            sort(addresses::reach::unlimit.begin(), addresses::reach::unlimit.end());
                            addresses::reach::unlimit.erase(
                                unique(addresses::reach::unlimit.begin(), addresses::reach::unlimit.end()),
                                addresses::reach::unlimit.end());

                        }


                        if (size_t d = std::count(memoryd.begin(), memoryd.end(), 3.0)) {
                            for (const size_t location : get_positions(memoryd, 3.0, d)) {
                                addresses::reach::doubles.push_back(address + location * sizeof(double));

                                sort(addresses::reach::doubles.begin(), addresses::reach::doubles.end());
                                addresses::reach::doubles.erase(
                                    unique(addresses::reach::doubles.begin(), addresses::reach::doubles.end()),
                                    addresses::reach::doubles.end());

                            }

                        }

                        if (size_t f = std::count(memoryf.begin(), memoryf.end(), 3.0f)) {
                            for (const size_t location : get_positions(memoryf, 3.0f, f)) {
                                addresses::reach::floats.push_back(address + location * sizeof(float));

                                sort(addresses::reach::floats.begin(), addresses::reach::floats.end());
                                addresses::reach::floats.erase(
                                    unique(addresses::reach::floats.begin(), addresses::reach::floats.end()),
                                    addresses::reach::floats.end());

                            }

                        }
                    }

                    if (address > 0x03000000) {
                        if (size_t x = std::count(memoryf.begin(), memoryf.end(), 2.5f)) {
                            for (const size_t location : get_positions(memoryf, 2.5f, x)) {
                                addresses::esp::locations.push_back(address + location * sizeof(float));
                                sort(addresses::esp::locations.begin(), addresses::esp::locations.end());
                                addresses::esp::locations.erase(
                                    unique(addresses::esp::locations.begin(), addresses::esp::locations.end()),
                                    addresses::esp::locations.end());
                            }
                        }
                    }

                    if (size_t x = std::count(memoryd.begin(), memoryd.end(), 16.0)) {
                        for (const size_t location : get_positions(memoryd, 16.0, x)) {
                            addresses::cavefinder::locations.push_back(address + location * sizeof(double));
                            sort(addresses::cavefinder::locations.begin(), addresses::cavefinder::locations.end());
                            addresses::cavefinder::locations.erase(unique(addresses::cavefinder::locations.begin(),
                                addresses::cavefinder::locations.end()),
                                addresses::cavefinder::locations.end());
                        }
                    }

                    if (size_t x = std::count(memoryd.begin(), memoryd.end(), 1000.0)) {
                        for (const size_t location : get_positions(memoryd, 1000.0, x)) {
                            addresses::timer::locations.push_back(address + location * sizeof(double));
                            sort(addresses::timer::locations.begin(), addresses::timer::locations.end());
                            addresses::timer::locations.erase(
                                unique(addresses::timer::locations.begin(), addresses::timer::locations.end()),
                                addresses::timer::locations.end());
                        }
                    }

                    if (size_t x = std::count(memoryd.begin(), memoryd.end(), 0.3)) {
                        for (const size_t location : get_positions(memoryd, 0.3, x)) {
                            addresses::bhop::locations.push_back(address + location * sizeof(double));
                            sort(addresses::bhop::locations.begin(), addresses::bhop::locations.end());
                            addresses::bhop::locations.erase(
                                unique(addresses::bhop::locations.begin(), addresses::bhop::locations.end()),
                                addresses::bhop::locations.end());
                        }
                    }

                    if (size_t x = std::count(memoryf.begin(), memoryf.end(), 0.02666667f)) {
                        for (const size_t location : get_positions(memoryf, 0.02666667f, x)) {
                            addresses::nametags::locations1.push_back(address + location * sizeof(float));
                            sort(addresses::nametags::locations1.begin(), addresses::nametags::locations1.end());
                            addresses::nametags::locations1.erase(unique(addresses::nametags::locations1.begin(),
                                addresses::nametags::locations1.end()),
                                addresses::nametags::locations1.end());
                        }
                    }

                    if (size_t x = std::count(memoryf.begin(), memoryf.end(), 0.16277136f)) {
                        for (const size_t location : get_positions(memoryf, 0.16277136f, x)) {
                            addresses::speed::locations.push_back(address + location * sizeof(float));
                            sort(addresses::speed::locations.begin(), addresses::speed::locations.end());
                            addresses::speed::locations.erase(unique(addresses::speed::locations.begin(),
                                addresses::speed::locations.end()),
                                addresses::speed::locations.end());
                        }
                    }

                    if (size_t x = std::count(memoryf.begin(), memoryf.end(), -0.02666667f)) {
                        for (const size_t location : get_positions(memoryf, -0.02666667f, x)) {
                            addresses::nametags::locations2.push_back(address + location * sizeof(float));

                            sort(addresses::nametags::locations2.begin(), addresses::nametags::locations2.end());
                            addresses::nametags::locations2.erase(unique(addresses::nametags::locations2.begin(),
                                addresses::nametags::locations2.end()),
                                addresses::nametags::locations2.end());
                        }
                    }

                    if (size_t x = std::count(memoryd.begin(), memoryd.end(), 0.08)) {
                        if (size_t x = std::count(memoryd.begin(), memoryd.end(), 0.9800000190734863)) {
                            for (const size_t location : get_positions(memoryd, 0.9800000190734863, x)) {
                                addresses::glide::locationsglide.push_back(address + location * sizeof(double));

                                sort(addresses::glide::locationsglide.begin(), addresses::glide::locationsglide.end());
                                addresses::glide::locationsglide.erase(
                                    unique(addresses::glide::locationsglide.begin(), addresses::glide::locationsglide.end()),
                                    addresses::glide::locationsglide.end());
                            }
                        }
                    }
                }
            }
        }
    }
}