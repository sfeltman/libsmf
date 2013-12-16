/*-
 * Copyright (c) 2007, 2008 Edward Tomasz Napierała <trasz@FreeBSD.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * ALTHOUGH THIS SOFTWARE IS MADE OF WIN AND SCIENCE, IT IS PROVIDED BY THE
 * AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 * \file
 *
 * Public interface declaration for libsmf, Standard MIDI File format library.
 */

/**
 *
 * \mainpage libsmf - general usage instructions
 *
 * An smf_t structure represents a "song".  Every valid smf contains one or more tracks.
 * Tracks contain zero or more events.  Libsmf doesn't care about actual MIDI data, as long
 * as it is valid from the MIDI specification point of view - it may be realtime message,
 * SysEx, whatever.
 * 
 * The only field in smf_t, smf_track_t, smf_event_t and smf_tempo_t structures your
 * code may modify is event->midi_buffer and event->midi_buffer_length.  Do not modify
 * other fields, _ever_.  You may read them, though.  Do not declare static instances
 * of these types, i.e. never do something like this:  "smf_t smf;".  Always use
 * "smf_t *smf = smf_new();".  The same applies to smf_track_t and smf_event_t.
 * 
 * Say you want to load a Standard MIDI File (.mid) file and play it back somehow.  This is (roughly)
 * how you do this:
 * 
 * \code
 * 	smf_t *smf;
 * 	smf_event_t *event;
 *
 * 	smf = smf_load(file_name);
 * 	if (smf == NULL) {
 * 		Whoops, something went wrong.
 * 		return;
 * 	}
 * 
 * 	while ((event = smf_get_next_event(smf)) != NULL) {
 *		if (smf_event_is_metadata(event))
 *			continue;
 * 
 * 		wait until event->time_seconds.
 * 		feed_to_midi_output(event->midi_buffer, event->midi_buffer_length);
 * 	}
 *
 *	smf_delete(smf);
 *
 * \endcode
 * 
 * Saving works like this:
 * 
 * \code
 *
 * 	smf_t *smf;
 *	smf_track_t *track;
 *	smf_event_t *event;
 *
 * 	smf = smf_new();
 * 	if (smf == NULL) {
 * 		Whoops.
 * 		return;
 * 	}
 * 
 * 	for (int i = 1; i <= number of tracks; i++) {
 * 		track = smf_track_new();
 * 		if (track == NULL) {
 * 			Whoops.
 * 			return;
 * 		}
 * 
 * 		smf_add_track(smf, track);
 * 
 * 		for (int j = 1; j <= number of events you want to put into this track; j++) {
 * 			event = smf_event_new_from_pointer(your MIDI message, message length);
 * 			if (event == NULL) {
 * 				Whoops.
 * 				return;
 * 			}
 * 
 * 			smf_track_add_event_seconds(track, event, seconds since start of the song);
 * 		}
 * 	}
 * 
 * 	ret = smf_save(smf, file_name);
 * 	if (ret) {
 * 		Whoops, saving failed for some reason.
 * 		return;
 * 	}
 *
 *	smf_delete(smf);
 *
 * \endcode
 *
 * There are two basic ways of getting MIDI data out of smf - sequential or by track/event number.  You may
 * mix them if you need to.  First one is used in the example above - seek to the point from which you want
 * the playback to start (using smf_seek_to_seconds(), smf_seek_to_pulses() or smf_seek_to_event()) and then
 * do smf_get_next_event() in loop, until it returns NULL.  Calling smf_load() causes the smf to be rewound
 * to the start of the song.
 *
 * Getting events by number works like this:
 *
 * \code
 *
 * smf_track_t *track = smf_get_track_by_number(smf, track_number);
 * smf_event_t *event = smf_track_get_event_by_number(track, event_number);
 *
 * \endcode
 *
 * To create new event, use smf_event_new(), smf_event_new_from_pointer() or smf_event_new_from_bytes().
 * First one creates an empty event - you need to manually allocate (using malloc(3)) buffer for
 * MIDI data, write MIDI data into it, put the address of that buffer into event->midi_buffer,
 * and the length of MIDI data into event->midi_buffer_length.  Note that deleting the event
 * (using smf_event_delete()) will free the buffer.
 *
 * Second form does most of this for you: it takes an address of the buffer containing MIDI data,
 * allocates storage and copies MIDI data into it.
 *
 * Third form is useful for manually creating short events, up to three bytes in length, for
 * example Note On or Note Off events.  It simply takes three bytes and creates MIDI event containing
 * them.  If you need to create MIDI message that takes only two bytes, pass -1 as the third byte.
 * For one byte message (System Realtime), pass -1 as second and third byte.
 *
 * To free an event, use smf_event_delete().
 *
 * To add event to the track, use smf_track_add_event_delta_pulses(), smf_track_add_event_pulses(),
 * or smf_track_add_event_seconds().  The difference between them is in the way you specify the time of
 * the event - with the first one, you specify it as an interval, in pulses, from the previous event
 * in this track; with the second one, you specify it as pulses from the start of the song, and with the
 * last one, you specify it as seconds from the start of the song.  Obviously, the first version can
 * only append events at the end of the track.
 *
 * To remove an event from the track it's attached to, use smf_event_remove_from_track().  You may
 * want to free the event (using smf_event_delete()) afterwards.
 *
 * To create new track, use smf_track_new().  To add track to the smf, use smf_add_track().
 * To remove track from its smf, use smf_track_remove_from_smf().  To free the track structure,
 * use smf_track_delete().
 *
 * Note that libsmf keeps things consistent.  If you free (using smf_track_delete()) a track that
 * is attached to an smf and contains events, libsmf will detach the events, free them, detach
 * the track, free it etc.
 *
 * Tracks and events are numbered consecutively, starting from one.  If you remove a track or event,
 * the rest of tracks/events will get renumbered.  To get the number of a given event in its track, use event->event_number.
 * To get the number of track in its smf, use track->track_number.  To get the number of events in the track,
 * use track->number_of_events.  To get the number of tracks in the smf, use smf->number_of_tracks.
 *
 * In SMF File Format, each track has to end with End Of Track metaevent.  If you load SMF file using smf_load(),
 * that will be the case.  If you want to create or edit an SMF, you don't need to worry about EOT events;
 * libsmf automatically takes care of them for you.  If you try to save an SMF with tracks that do not end
 * with EOTs, smf_save() will append them.  If you try to add event that happens after EOT metaevent, libsmf
 * will remove the EOT.  If you want to add EOT manually, you can, of course, using smf_track_add_eot_seconds()
 * or smf_track_add_eot_pulses().
 *
 * Each event carries three time values - event->time_seconds, which is seconds since the start of the song,
 * event->time_pulses, which is PPQN clocks since the start of the song, and event->delta_pulses, which is PPQN clocks
 * since the previous event in that track.  These values are invalid if the event is not attached to the track.
 * If event is attached, all three values are valid.  Time of the event is specified when adding the event
 * (using smf_track_add_event_seconds(), smf_track_add_event_pulses() or smf_track_add_event_delta_pulses()); the remaining
 * two values are computed from that.
 *
 * Tempo related stuff happens automatically - when you add a metaevent that
 * is Tempo Change or Time Signature, libsmf adds that event to the tempo map.  If you remove
 * Tempo Change event that is in the middle of the song, the rest of the events will have their
 * event->time_seconds recomputed from event->time_pulses before smf_event_remove_from_track() function returns.
 * Adding Tempo Change in the middle of the song works in a similar way.
 * 	
 * MIDI data (event->midi_buffer) is always kept in normalized form - it always begins with status byte
 * (no running status), there are no System Realtime events embedded in them etc.  Events like SysExes
 * are in "on the wire" form, without embedded length that is used in SMF file format.  Obviously
 * libsmf "normalizes" MIDI data during loading and "denormalizes" (adding length to SysExes, escaping
 * System Common and System Realtime messages etc) during writing.
 *
 * Note that you always have to first add the track to smf, and then add events to the track.
 * Doing it the other way around will trip asserts.  Also, try to add events at the end of the track and remove
 * them from the end of the track, that's much more efficient.
 * 
 * All the libsmf functions have prefix "smf_".  First argument for routines whose names start with
 * "smf_event_" is "smf_event_t *", for routines whose names start with "smf_track_" - "smf_track_t *",
 * and for plain "smf_" - "smf_t *".  The only exception are smf_whatever_new routines.
 * Library does not use any global variables and is thread-safe,
 * as long as you don't try to work on the same SMF (smf_t and its descendant tracks and events) from several
 * threads at once without protecting it with mutex.  Library depends on glib and nothing else.  License is
 * BSD, two clause, which basically means you can use it freely in your software, both Open Source (including
 * GPL) and closed source.
 *
 */

#ifndef SMF_H
#define SMF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <glib.h>


typedef struct _SmfFile  SmfFile;
typedef struct _SmfTempo SmfTempo;
typedef struct _SmfTrack SmfTrack;
typedef struct _SmfEvent SmfEvent;


/** Represents a "song", that is, collection of one or more tracks. */
struct _SmfFile {
	int		format;

	/** These fields are extracted from "division" field of MThd header.  Valid is _either_ ppqn or frames_per_second/resolution. */
	int		ppqn;
	int		frames_per_second;
	int		resolution;
	int		number_of_tracks;

	/** These are private fields using only by loading and saving routines. */
	FILE		*stream;
	void		*file_buffer;
	int		file_buffer_length;
	int		next_chunk_offset;
	int		expected_number_of_tracks;

	/** Private, used by smf.c. */
	GPtrArray	*tracks_array;
	double		last_seek_position;

	/** Private, used by smf_tempo.c. */
	/** Array of pointers to smf_tempo_struct. */
	GPtrArray	*tempo_array;
	int		ref_count;
};

/* Routines for manipulating SmfFile. */
SmfFile *smf_file_new(void) G_GNUC_WARN_UNUSED_RESULT;
SmfFile *smf_file_ref(SmfFile *smf) G_GNUC_WARN_UNUSED_RESULT;
void smf_file_unref(SmfFile *smf);

int smf_file_set_format(SmfFile *smf, int format) G_GNUC_WARN_UNUSED_RESULT;
int smf_file_set_ppqn(SmfFile *smf, int ppqn) G_GNUC_WARN_UNUSED_RESULT;

char *smf_file_decode(const SmfFile *smf) G_GNUC_WARN_UNUSED_RESULT;

SmfTrack *smf_file_get_track_by_number(const SmfFile *smf, int track_number) G_GNUC_WARN_UNUSED_RESULT;

SmfEvent *smf_file_peek_next_event(SmfFile *smf) G_GNUC_WARN_UNUSED_RESULT;
SmfEvent *smf_file_get_next_event(SmfFile *smf) G_GNUC_WARN_UNUSED_RESULT;
void smf_file_skip_next_event(SmfFile *smf);

void smf_file_rewind(SmfFile *smf);
int smf_file_seek_to_seconds(SmfFile *smf, double seconds) G_GNUC_WARN_UNUSED_RESULT;
int smf_file_seek_to_pulses(SmfFile *smf, int pulses) G_GNUC_WARN_UNUSED_RESULT;
int smf_file_seek_to_event(SmfFile *smf, const SmfEvent *target) G_GNUC_WARN_UNUSED_RESULT;

int smf_file_get_length_pulses(const SmfFile *smf) G_GNUC_WARN_UNUSED_RESULT;
double smf_file_get_length_seconds(const SmfFile *smf) G_GNUC_WARN_UNUSED_RESULT;

void smf_file_add_track(SmfFile *smf, SmfTrack *track);
void smf_file_remove_track(SmfFile *smf, SmfTrack *track);

/* Routines for loading SMF files. */
SmfFile *smf_file_load(const char *file_name) G_GNUC_WARN_UNUSED_RESULT;
SmfFile *smf_file_load_from_memory(const void *buffer, const int buffer_length) G_GNUC_WARN_UNUSED_RESULT;

/* Routine for writing SMF files. */
int smf_file_save(SmfFile *smf, const char *file_name) G_GNUC_WARN_UNUSED_RESULT;

/* Routines for manipulating SmfTempo. */
SmfTempo *smf_file_get_tempo_by_pulses(const SmfFile *smf, int pulses) G_GNUC_WARN_UNUSED_RESULT;
SmfTempo *smf_file_get_tempo_by_seconds(const SmfFile *smf, double seconds) G_GNUC_WARN_UNUSED_RESULT;
SmfTempo *smf_file_get_tempo_by_number(const SmfFile *smf, int number) G_GNUC_WARN_UNUSED_RESULT;
SmfTempo *smf_file_get_last_tempo(const SmfFile *smf) G_GNUC_WARN_UNUSED_RESULT;

/** Represents a single track. */
struct _SmfTrack {
	SmfFile		*smf;

	int		track_number;
	int		number_of_events;

	/** These are private fields using only by loading and saving routines. */
	void		*file_buffer;
	int		file_buffer_length;
	int		last_status; /* Used for "running status". */

	/** Private, used by smf.c. */
	/** Offset into buffer, used in parse_next_event(). */
	int		next_event_offset;
	int		next_event_number;

	/** Absolute time of next event on events_queue. */
	int		time_of_next_event;
	GPtrArray	*events_array;

	/** API consumer is free to use this for whatever purpose.  NULL in freshly allocated track.
	    Note that tracks might be deallocated not only explicitly, by calling smf_track_delete(),
	    but also implicitly, e.g. when calling smf_delete() with tracks still added to
	    the smf; there is no mechanism for libsmf to notify you about removal of the track. */
	void		*user_pointer;

	int		ref_count;
};

/* Routines for manipulating SmfTrack. */
SmfTrack *smf_track_new(void) G_GNUC_WARN_UNUSED_RESULT;
void smf_track_delete(SmfTrack *track);
SmfTrack *smf_track_ref(SmfTrack *track) G_GNUC_WARN_UNUSED_RESULT;
void smf_track_unref(SmfTrack *track);

SmfEvent *smf_track_get_next_event(SmfTrack *track) G_GNUC_WARN_UNUSED_RESULT;
SmfEvent *smf_track_get_event_by_number(const SmfTrack *track, int event_number) G_GNUC_WARN_UNUSED_RESULT;
SmfEvent *smf_track_get_last_event(const SmfTrack *track) G_GNUC_WARN_UNUSED_RESULT;

void smf_track_add_event_delta_pulses(SmfTrack *track, SmfEvent *event, int delta);
void smf_track_add_event_pulses(SmfTrack *track, SmfEvent *event, int pulses);
void smf_track_add_event_seconds(SmfTrack *track, SmfEvent *event, double seconds);
int smf_track_add_eot_delta_pulses(SmfTrack *track, int delta) G_GNUC_WARN_UNUSED_RESULT;
int smf_track_add_eot_pulses(SmfTrack *track, int pulses) G_GNUC_WARN_UNUSED_RESULT;
int smf_track_add_eot_seconds(SmfTrack *track, double seconds) G_GNUC_WARN_UNUSED_RESULT;
void smf_track_remove_event(SmfTrack *track, SmfEvent *event);

/** Represents a single MIDI event or metaevent. */
struct _SmfEvent {
	/** Pointer to the track, or NULL if event is not attached. */
	SmfTrack	*track;

	/** Number of this event in the track.  Events are numbered consecutively, starting from one. */
	int		event_number;

	/** Note that the time fields are invalid, if event is not attached to a track. */
	/** Time, in pulses, since the previous event on this track. */
	int		delta_time_pulses;

	/** Time, in pulses, since the start of the song. */
	int		time_pulses;

	/** Time, in seconds, since the start of the song. */
	double		time_seconds;

	/** Tracks are numbered consecutively, starting from 1. */
	int		track_number;

	/** Pointer to the buffer containing MIDI message.  This is freed by smf_event_delete. */
	unsigned char	*midi_buffer;

	/** Length of the MIDI message in the buffer, in bytes. */
	int		midi_buffer_length; 

	/** API consumer is free to use this for whatever purpose.  NULL in freshly allocated event.
	    Note that events might be deallocated not only explicitly, by calling smf_event_delete(),
	    but also implicitly, e.g. when calling smf_track_delete() with events still added to
	    the track; there is no mechanism for libsmf to notify you about removal of the event. */
	void		*user_pointer;
	int		ref_count;
};

/* Routines for manipulating SmfEvent. */
SmfEvent *smf_event_new(void) G_GNUC_WARN_UNUSED_RESULT;
SmfEvent *smf_event_new_from_pointer(void *midi_data, int len) G_GNUC_WARN_UNUSED_RESULT;
SmfEvent *smf_event_new_from_bytes(int first_byte, int second_byte, int third_byte) G_GNUC_WARN_UNUSED_RESULT;
SmfEvent *smf_event_new_textual(int type, const char *text) G_GNUC_WARN_UNUSED_RESULT;
void smf_event_delete(SmfEvent *event);
void smf_event_remove_from_track(SmfEvent *event);
SmfEvent *smf_event_ref(SmfEvent *event) G_GNUC_WARN_UNUSED_RESULT;
void smf_event_unref(SmfEvent *event);

int smf_event_is_last(const SmfEvent *event) G_GNUC_WARN_UNUSED_RESULT;
int smf_event_is_valid(const SmfEvent *event) G_GNUC_WARN_UNUSED_RESULT;
int smf_event_is_metadata(const SmfEvent *event) G_GNUC_WARN_UNUSED_RESULT;
int smf_event_is_system_realtime(const SmfEvent *event) G_GNUC_WARN_UNUSED_RESULT;
int smf_event_is_system_common(const SmfEvent *event) G_GNUC_WARN_UNUSED_RESULT;
int smf_event_is_sysex(const SmfEvent *event) G_GNUC_WARN_UNUSED_RESULT;
int smf_event_is_eot(const SmfEvent *event) G_GNUC_WARN_UNUSED_RESULT;
int smf_event_is_textual(const SmfEvent *event) G_GNUC_WARN_UNUSED_RESULT;
char *smf_event_decode(const SmfEvent *event) G_GNUC_WARN_UNUSED_RESULT;
char *smf_event_extract_text(const SmfEvent *event) G_GNUC_WARN_UNUSED_RESULT;


/** Describes a single tempo or time signature change. */
struct _SmfTempo {
	int time_pulses;
	double time_seconds;
	int microseconds_per_quarter_note;
	int numerator;
	int denominator;
	int clocks_per_click;
	int notes_per_note;
	int ref_count;
};

/* Routines for manipulating SmfTempo. */
SmfTempo *smf_tempo_ref(SmfTempo *tempo) G_GNUC_WARN_UNUSED_RESULT;
void smf_tempo_unref(SmfTempo *tempo);

const char *smf_get_version(void) G_GNUC_WARN_UNUSED_RESULT;

/* Backwards compatable API/ABI */

typedef SmfFile   		smf_t;
typedef SmfTempo  		smf_tempo_t;
typedef SmfTrack  		smf_track_t;
typedef SmfEvent  		smf_event_t;

G_DEPRECATED_FOR(smf_file_new)
smf_t *smf_new(void) G_GNUC_WARN_UNUSED_RESULT;

G_DEPRECATED_FOR(smf_file_unref)
void smf_delete(smf_t *smf);

G_DEPRECATED_FOR(smf_file_set_format)
int smf_set_format(smf_t *smf, int format) G_GNUC_WARN_UNUSED_RESULT;

G_DEPRECATED_FOR(smf_file_set_ppqn)
int smf_set_ppqn(smf_t *smf, int format) G_GNUC_WARN_UNUSED_RESULT;

G_DEPRECATED_FOR(smf_file_decode)
char *smf_decode(const smf_t *smf) G_GNUC_WARN_UNUSED_RESULT;

G_DEPRECATED_FOR(smf_file_get_track_by_number)
smf_track_t *smf_get_track_by_number(const smf_t *smf, int track_number) G_GNUC_WARN_UNUSED_RESULT;

G_DEPRECATED_FOR(smf_file_peek_next_event)
smf_event_t *smf_peek_next_event(smf_t *smf) G_GNUC_WARN_UNUSED_RESULT;

G_DEPRECATED_FOR(smf_file_get_next_event)
smf_event_t *smf_get_next_event(smf_t *smf) G_GNUC_WARN_UNUSED_RESULT;

G_DEPRECATED_FOR(smf_file_skip_next_event)
void smf_skip_next_event(smf_t *smf);

G_DEPRECATED_FOR(smf_file_rewind)
void smf_rewind(smf_t *smf);

G_DEPRECATED_FOR(smf_file_seek_to_seconds)
int smf_seek_to_seconds(smf_t *smf, double seconds) G_GNUC_WARN_UNUSED_RESULT;

G_DEPRECATED_FOR(smf_file_seek_to_pulses)
int smf_seek_to_pulses(smf_t *smf, int pulses) G_GNUC_WARN_UNUSED_RESULT;

G_DEPRECATED_FOR(smf_file_seek_to_event)
int smf_seek_to_event(smf_t *smf, const smf_event_t *event) G_GNUC_WARN_UNUSED_RESULT;

G_DEPRECATED_FOR(smf_file_get_length_pulses)
int smf_get_length_pulses(const smf_t *smf) G_GNUC_WARN_UNUSED_RESULT;

G_DEPRECATED_FOR(smf_file_get_length_seconds)
double smf_get_length_seconds(const smf_t *smf) G_GNUC_WARN_UNUSED_RESULT;

G_DEPRECATED_FOR(smf_file_add_track)
void smf_add_track(smf_t *smf, smf_track_t *track);

G_DEPRECATED_FOR(smf_file_load)
smf_t *smf_load(const char *file_name) G_GNUC_WARN_UNUSED_RESULT;

G_DEPRECATED_FOR(smf_file_load_from_memory)
smf_t *smf_load_from_memory(const void *buffer, const int buffer_length) G_GNUC_WARN_UNUSED_RESULT;

G_DEPRECATED_FOR(smf_file_save)
int smf_save(smf_t *smf, const char *file_name) G_GNUC_WARN_UNUSED_RESULT;

G_DEPRECATED_FOR(smf_file_get_tempo_by_pulses)
smf_tempo_t *smf_get_tempo_by_pulses(const smf_t *smf, int pulses) G_GNUC_WARN_UNUSED_RESULT;

G_DEPRECATED_FOR(smf_file_get_tempo_by_seconds)
smf_tempo_t *smf_get_tempo_by_seconds(const smf_t *smf, double seconds) G_GNUC_WARN_UNUSED_RESULT;

G_DEPRECATED_FOR(smf_file_get_tempo_by_number)
smf_tempo_t *smf_get_tempo_by_number(const smf_t *smf, int number) G_GNUC_WARN_UNUSED_RESULT;

G_DEPRECATED_FOR(smf_file_get_last_tempo)
smf_tempo_t *smf_get_last_tempo(const smf_t *smf) G_GNUC_WARN_UNUSED_RESULT;

#ifdef __cplusplus
}
#endif

#endif /* SMF_H */

