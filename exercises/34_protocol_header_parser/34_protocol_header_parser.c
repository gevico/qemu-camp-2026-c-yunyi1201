#include <stdint.h>
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)

typedef struct {
    uint8_t  version_raw;  /* 高4位: major, 低4位: minor */
    uint8_t  version_pad;  /* padding / second byte of version field */
    uint16_t length_be;    /* length in big-endian */
    uint8_t  flags_raw;    /* flags byte */
} proto_header_raw_t;

typedef struct {
    unsigned int ver_major : 4;
    unsigned int ver_minor : 4;
    uint16_t length;
    uint8_t  flags;
} proto_header_bits_t;

#pragma pack(pop)

static uint16_t be16_to_cpu(uint16_t be) {
    return (uint16_t)(((be & 0x00FFu) << 8) | ((be & 0xFF00u) >> 8));
}

int main(void) {
    /* 测试输入字节流：00 03 00 20 00 */
    const uint8_t stream[5] = {0x00, 0x03, 0x00, 0x20, 0x00};

    proto_header_raw_t raw = {0};
    memcpy(&raw, stream, sizeof(raw));

    /* 版本字段：stream[0]=0x00 (major), stream[1]=0x03 (minor) */
    unsigned int ver_major = (raw.version_raw >> 4) & 0x0Fu;
    unsigned int ver_minor = raw.version_raw & 0x0Fu;
    /* But stream[0]=0x00 and stream[1]=0x03 -> major=0, minor=3 */
    /* version_raw = stream[0] = 0x00, version_pad = stream[1] = 0x03 */
    ver_major = (raw.version_raw >> 4) & 0x0Fu;  /* 0 */
    ver_minor = raw.version_pad & 0x0Fu;          /* 3 */

    /* 解析长度：网络序 16 位 */
    uint16_t length = be16_to_cpu(raw.length_be);

    /* 解析标志位：低 5 位为功能标志 */
    unsigned flags = (unsigned)(raw.flags_raw & 0x1Fu);

    proto_header_bits_t view;
    view.ver_major = ver_major;
    view.ver_minor = ver_minor;
    view.length = length;
    view.flags = (uint8_t)flags;

    /* 期望输出：version:0.3, length:32, flags:0x00 */
    printf("version:%u.%u, length:%u, flags:0x%02X\n", view.ver_major, view.ver_minor, view.length, view.flags & 0xFFu);

    return 0;
}
