#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Mon Sep 20 12:59:43 2021
##################################################

from distutils.version import StrictVersion

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

from PyQt5 import Qt
from PyQt5 import Qt, QtCore
from gnuradio import analog
from gnuradio import blocks
from gnuradio import channels
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import iio
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from gnuradio.qtgui import Range, RangeWidget
from optparse import OptionParser
import sip
import sys
from gnuradio import qtgui


class top_block(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Top Block")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Top Block")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "top_block")

        if StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
            self.restoreGeometry(self.settings.value("geometry").toByteArray())
        else:
            self.restoreGeometry(self.settings.value("geometry", type=QtCore.QByteArray))

        ##################################################
        # Variables
        ##################################################
        self.scale = scale = 1
        self.samp_rate_pluro = samp_rate_pluro = 3500000
        self.samp_rate = samp_rate = 256000
        self.noiverVoltage = noiverVoltage = 0
        self.noise_amp = noise_amp = 0
        self.freq_offset = freq_offset = 0
        self.freq_cos = freq_cos = 25000
        self.freqOffset = freqOffset = 0
        self.freq = freq = 2600000000

        ##################################################
        # Blocks
        ##################################################
        self._noise_amp_range = Range(0, 100000, 100, 0, 200)
        self._noise_amp_win = RangeWidget(self._noise_amp_range, self.set_noise_amp, 'Channel Noise', "counter_slider", float)
        self.top_grid_layout.addWidget(self._noise_amp_win, 1, 2, 1, 1)
        [self.top_grid_layout.setRowStretch(r,1) for r in range(1,2)]
        [self.top_grid_layout.setColumnStretch(c,1) for c in range(2,3)]
        self._freq_offset_range = Range(-.5, .5, .01, 0, 200)
        self._freq_offset_win = RangeWidget(self._freq_offset_range, self.set_freq_offset, 'Frequency Offset', "counter_slider", float)
        self.top_grid_layout.addWidget(self._freq_offset_win, 2, 2, 1, 1)
        [self.top_grid_layout.setRowStretch(r,1) for r in range(2,3)]
        [self.top_grid_layout.setColumnStretch(c,1) for c in range(2,3)]
        self._freq_cos_range = Range(-50000, 50000, 200, 25000, 200)
        self._freq_cos_win = RangeWidget(self._freq_cos_range, self.set_freq_cos, 'Frequency Cos', "counter_slider", float)
        self.top_layout.addWidget(self._freq_cos_win)
        self.qtgui_sink_x_0_1 = qtgui.sink_c(
        	1024, #fftsize
        	firdes.WIN_BLACKMAN_hARRIS, #wintype
        	0, #fc
        	samp_rate, #bw
        	"Tx", #name
        	True, #plotfreq
        	True, #plotwaterfall
        	True, #plottime
        	True, #plotconst
        )
        self.qtgui_sink_x_0_1.set_update_time(1.0/10)
        self._qtgui_sink_x_0_1_win = sip.wrapinstance(self.qtgui_sink_x_0_1.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_sink_x_0_1_win)

        self.qtgui_sink_x_0_1.enable_rf_freq(False)



        self.qtgui_sink_x_0 = qtgui.sink_c(
        	1024, #fftsize
        	firdes.WIN_BLACKMAN_hARRIS, #wintype
        	0, #fc
        	samp_rate, #bw
        	"Tx", #name
        	True, #plotfreq
        	True, #plotwaterfall
        	True, #plottime
        	True, #plotconst
        )
        self.qtgui_sink_x_0.set_update_time(1.0/10)
        self._qtgui_sink_x_0_win = sip.wrapinstance(self.qtgui_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_sink_x_0_win)

        self.qtgui_sink_x_0.enable_rf_freq(False)



        self.pluto_source_0 = iio.pluto_source('usb:1.67.5', freq, samp_rate_pluro, 1 - 1, 20000000, 0x8000, True, True, True, "manual", 64.0, '', True)
        self.channels_channel_model_0_0 = channels.channel_model(
        	noise_voltage=noise_amp,
        	frequency_offset=freq_offset,
        	epsilon=1.0,
        	taps=(1.0 + 1.0j*0, ),
        	noise_seed=0,
        	block_tags=False
        )
        self.blocks_multiply_const_vxx_0_0 = blocks.multiply_const_vcc((1.0/32768, ))
        self.blocks_int_to_float_0_0 = blocks.int_to_float(1, 1)
        self.blocks_int_to_float_0 = blocks.int_to_float(1, 1)
        self.blocks_float_to_complex_0 = blocks.float_to_complex(1)
        self.blocks_file_source_0_0 = blocks.file_source(gr.sizeof_int*1, '/home/user/Public/gnuRadioFSK_data_q.txt', True)
        self.blocks_file_source_0 = blocks.file_source(gr.sizeof_int*1, '/home/user/Public/gnuRadioFSK_data_i.txt', True)
        self.blocks_add_xx_1 = blocks.add_vcc(1)
        self.analog_sig_source_x_1 = analog.sig_source_c(100000, analog.GR_COS_WAVE, freq_cos, 10000, 0)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_sig_source_x_1, 0), (self.blocks_add_xx_1, 0))
        self.connect((self.blocks_add_xx_1, 0), (self.channels_channel_model_0_0, 0))
        self.connect((self.blocks_file_source_0, 0), (self.blocks_int_to_float_0, 0))
        self.connect((self.blocks_file_source_0_0, 0), (self.blocks_int_to_float_0_0, 0))
        self.connect((self.blocks_float_to_complex_0, 0), (self.blocks_add_xx_1, 1))
        self.connect((self.blocks_int_to_float_0, 0), (self.blocks_float_to_complex_0, 0))
        self.connect((self.blocks_int_to_float_0_0, 0), (self.blocks_float_to_complex_0, 1))
        self.connect((self.blocks_multiply_const_vxx_0_0, 0), (self.qtgui_sink_x_0, 0))
        self.connect((self.channels_channel_model_0_0, 0), (self.blocks_multiply_const_vxx_0_0, 0))
        self.connect((self.pluto_source_0, 0), (self.qtgui_sink_x_0_1, 0))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_scale(self):
        return self.scale

    def set_scale(self, scale):
        self.scale = scale

    def get_samp_rate_pluro(self):
        return self.samp_rate_pluro

    def set_samp_rate_pluro(self, samp_rate_pluro):
        self.samp_rate_pluro = samp_rate_pluro
        self.pluto_source_0.set_params(self.freq, self.samp_rate_pluro, 20000000, True, True, True, "manual", 64.0, '', True)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.qtgui_sink_x_0_1.set_frequency_range(0, self.samp_rate)
        self.qtgui_sink_x_0.set_frequency_range(0, self.samp_rate)

    def get_noiverVoltage(self):
        return self.noiverVoltage

    def set_noiverVoltage(self, noiverVoltage):
        self.noiverVoltage = noiverVoltage

    def get_noise_amp(self):
        return self.noise_amp

    def set_noise_amp(self, noise_amp):
        self.noise_amp = noise_amp
        self.channels_channel_model_0_0.set_noise_voltage(self.noise_amp)

    def get_freq_offset(self):
        return self.freq_offset

    def set_freq_offset(self, freq_offset):
        self.freq_offset = freq_offset
        self.channels_channel_model_0_0.set_frequency_offset(self.freq_offset)

    def get_freq_cos(self):
        return self.freq_cos

    def set_freq_cos(self, freq_cos):
        self.freq_cos = freq_cos
        self.analog_sig_source_x_1.set_frequency(self.freq_cos)

    def get_freqOffset(self):
        return self.freqOffset

    def set_freqOffset(self, freqOffset):
        self.freqOffset = freqOffset

    def get_freq(self):
        return self.freq

    def set_freq(self, freq):
        self.freq = freq
        self.pluto_source_0.set_params(self.freq, self.samp_rate_pluro, 20000000, True, True, True, "manual", 64.0, '', True)


def main(top_block_cls=top_block, options=None):

    if StrictVersion("4.5.0") <= StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.aboutToQuit.connect(quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
