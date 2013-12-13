/* ABI compatibility with older versions. */

#include "smf.h"

smf_t *smf_new(void) {
    return smf_file_new();
}

void smf_delete(smf_t *smf) {
    smf_file_delete(smf);
}

int smf_set_format(smf_t *smf, int format) {
    return smf_file_set_format(smf, format);
}

int smf_set_ppqn(smf_t *smf, int format) {
    return smf_file_set_ppqn(smf, format);
}

char *smf_decode(const smf_t *smf) {
    return smf_file_decode(smf);
}

smf_track_t *smf_get_track_by_number(const smf_t *smf, int track_number) {
    return smf_file_get_track_by_number(smf, track_number);
}

smf_event_t *smf_peek_next_event(smf_t *smf) {
    return smf_file_peek_next_event(smf);
}

smf_event_t *smf_get_next_event(smf_t *smf) {
    return smf_file_get_next_event(smf);
}

void smf_skip_next_event(smf_t *smf) {
    smf_file_skip_next_event(smf);
}

void smf_rewind(smf_t *smf) {
    smf_file_rewind(smf);
}

int smf_seek_to_seconds(smf_t *smf, double seconds) {
    return smf_file_seek_to_seconds(smf, seconds);
}

int smf_seek_to_pulses(smf_t *smf, int pulses) {
    return smf_file_seek_to_pulses(smf, pulses);
}

int smf_seek_to_event(smf_t *smf, const smf_event_t *event) {
    return smf_file_seek_to_event(smf, event);
}

int smf_get_length_pulses(const smf_t *smf) {
    return smf_file_get_length_pulses(smf);
}

double smf_get_length_seconds(const smf_t *smf) {
    return smf_file_get_length_seconds(smf);
}

void smf_add_track(smf_t *smf, smf_track_t *track) {
    smf_file_add_track(smf, track);
}

smf_t *smf_load(const char *file_name) {
    return smf_file_load(file_name);
}

smf_t *smf_load_from_memory(const void *buffer, const int buffer_length) {
    return smf_file_load_from_memory(buffer, buffer_length);
}

int smf_save(smf_t *smf, const char *file_name) {
    return smf_file_save(smf, file_name);
}

smf_tempo_t *smf_get_tempo_by_pulses(const smf_t *smf, int pulses) {
    return smf_file_get_tempo_by_pulses(smf, pulses);
}

smf_tempo_t *smf_get_tempo_by_seconds(const smf_t *smf, double seconds) {
    return smf_get_tempo_by_seconds(smf, seconds);
}
smf_tempo_t *smf_get_tempo_by_number(const smf_t *smf, int number) {
    return smf_file_get_tempo_by_number(smf, number);
}

smf_tempo_t *smf_get_last_tempo(const smf_t *smf) {
    return smf_file_get_last_tempo(smf);
}
