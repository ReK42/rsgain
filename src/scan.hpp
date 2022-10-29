#ifndef HAS_SCAN_H
#define HAS_SCAN_H

#include <mutex>
#include <vector>
#include <filesystem>
#include <ebur128.h>
#include "scan.hpp"

void free_ebur128(ebur128_state *ebur128);

enum class FileType {
    INVALID = -1,
    MP2,
    MP3,
    FLAC,
    OGG,
    OPUS,
    M4A,
    WMA,
    WAV,
    AIFF,
    WAVPACK,
    APE,
	MPC
};

struct ScanResult {
	double track_gain;
	double track_peak;
	double track_loudness;

	double album_gain;
	double album_peak;
	double album_loudness;
};

struct ScanData {
    unsigned int files;
	unsigned int skipped = 0;
    unsigned int clipping_adjustments;
    double total_gain;
    double total_peak;
    unsigned int total_negative;
    unsigned int total_positive;
    std::vector<std::string> error_directories;

    ScanData(void) : files(0), clipping_adjustments(0), total_gain(0.f), total_peak(0.f), total_positive(0), total_negative(0){};
};

struct Track{
	std::string path;
	FileType type;
	std::string container;
	ScanResult result;
	int codec_id;
	std::unique_ptr<ebur128_state, void (*)(ebur128_state*)> ebur128;
	bool tclip;
	bool aclip;

	Track(const std::string &path, FileType type) : path(path), type(type), ebur128(NULL, free_ebur128), tclip(false), aclip(false) {};
	bool scan(const Config &config, std::mutex *ffmpeg_mutex);
	bool calculate_loudness(const Config &config);
};

class ScanJob {
	private:
		std::vector<Track> tracks;

		void calculate_loudness(const Config &config);
		void calculate_album_loudness(const Config &config);
		void tag_tracks(const Config &config);

	public:
		FileType type;
		std::string path;
		int nb_files;
		bool error;
		int clipping_adjustments;
		unsigned int skipped = 0;

		ScanJob() : nb_files(0), error(false), clipping_adjustments(0) {};
		bool add_files(char **files, int nb_files);
		FileType add_directory(std::filesystem::path &path);
		bool scan(const Config &config, std::mutex *ffmpeg_mutex = NULL);
		void update_data(ScanData &data);
};

#endif