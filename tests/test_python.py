import os
import unittest
import tempfile

from gi.repository import Smf


class Test(unittest.TestCase):
    def setUp(self):
        self.path = os.path.dirname(__file__)

    def compare_smf_files(self, a, b):
        self.assertEqual(a.format, b.format)
        self.assertEqual(a.ppqn, b.ppqn)
        self.assertEqual(a.frames_per_second, b.frames_per_second)
        self.assertEqual(a.resolution, b.resolution)
        self.assertEqual(a.number_of_tracks, b.number_of_tracks)
        self.assertEqual(len(a.tracks_array), len(b.tracks_array))

        for i in range(a.number_of_tracks):
            tracka = a.tracks_array[i]
            trackb = b.tracks_array[i]

            self.assertEqual(tracka.smf, a)
            self.assertEqual(trackb.smf, b)

            self.assertEqual(tracka.track_number, trackb.track_number)
            self.assertEqual(tracka.number_of_events, trackb.number_of_events)
            self.assertEqual(tracka.file_buffer_length, trackb.file_buffer_length)
            self.assertEqual(tracka.last_status, trackb.last_status)
            self.assertEqual(tracka.next_event_offset, trackb.next_event_offset)
            #self.assertEqual(tracka.next_event_number, trackb.next_event_number)
            #self.assertEqual(tracka.time_of_next_event, trackb.time_of_next_event)

            tracka_events = tracka.events_array
            trackb_events = trackb.events_array

            for j in range(tracka.number_of_events):
                eventa = tracka_events[j]
                eventb = trackb_events[j]

                self.assertEqual(tracka, eventa.track)
                self.assertEqual(trackb, eventb.track)

                self.assertEqual(eventa.event_number, eventb.event_number)
                self.assertEqual(eventa.delta_time_pulses, eventb.delta_time_pulses)
                self.assertEqual(eventa.time_pulses, eventb.time_pulses)
                self.assertEqual(eventa.time_seconds, eventb.time_seconds)
                self.assertEqual(eventa.track_number, eventb.track_number)
                self.assertEqual(eventa.midi_buffer_length, eventb.midi_buffer_length)
                self.assertEqual(eventa.get_buffer(), eventb.get_buffer())

    @unittest.expectedFailure
    def test_tempo_ref_counts(self):
        bach = Smf.File.load(os.path.join(self.path, 'chpn_op53.mid'))
        tempo = bach.get_last_tempo()
        #self.assertEqual(tempo.ref_count, 2)
        bach.remove_tempo(tempo)
        self.assertEqual(tempo.ref_count, 1)

    def test_file_ref_count(self):
        pass

    def test_bach_read_write_read_compare(self):
        orig = Smf.File.load(os.path.join(self.path, 'chpn_op53.mid'))

        handle, temp_filename = tempfile.mkstemp('mid')
        os.close(handle)
        orig.save(temp_filename)
        new = Smf.File.load(temp_filename)
        self.compare_smf_files(orig, new)


if __name__ == '__main__':
    unittest.main()

