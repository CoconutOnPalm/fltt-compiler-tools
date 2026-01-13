#pragma once

#include <filesystem>
#include <fstream>
#include <vector>
#include <thread>
#include <set>

#include "../Internal/EngineLogging.hpp"

namespace ke
{
	/**
	 * @class FileReader.
	 */
	class FileReader
	{
	private:

		std::mutex m_imutex; // in case my dumb ass ever decided to multithread this

		std::ifstream m_file;
		bool m_ready = false;

		static inline std::set<std::string> s_supported_file_types;


	public:

		FileReader() = default;

		FileReader(const std::filesystem::path& path)
		{
			open(path);
		}

		~FileReader()
		{
			std::lock_guard lock(m_imutex);

			m_file.close();
		}

		/**
		 * @brief Opens a file. Returns true if successful.
		 * 
		 * @param path
		 * @return True if successful. False otherwise.
		 */
		bool open(const std::filesystem::path& path)
		{
			std::lock_guard lock(m_imutex);

			m_ready = false;

			if (m_file.is_open())
			{
				m_file.close();
			}

			m_file.open(path);


			if (m_file.is_open())
			{
				m_ready = true;
				return true;
			}
			else
			{
				std::string reason = "Unknown reason.";

				if (!std::filesystem::exists(path))
					reason = "file does not exist.";
				else if (std::filesystem::is_directory(path))
					reason = "file is a directory.";
				else if (std::filesystem::is_empty(path))
					reason = "file is empty.";
				else if (s_supported_file_types.find(path.extension().string()) == s_supported_file_types.end())
					reason = "file type is not supported.";

				ke::_internal::EngineLog::Error("Failed to open file: \"{0}\". Reason: {1}", path.string(), reason);

				return false;
			}
		}

		/**
		 * @brief Returns true if the file opened successfully.
		 * 
		 * @return True if the file is open.
		 */
		bool isOpen() const { return m_ready; }

		/**
		 * @brief Closes the file.
		 */
		void close()
		{
			std::lock_guard lock(m_imutex);

			m_file.close();
		}

		/**
		 * @brief Reads the entire file line by line and returns it as a vector of strings. Skips empty lines and comments.
		 * 
		 * @param remove_BOM If true, removes the Byt Order Marks (BOM) from the first line, or does nothing if file has no BOM. If false, does nothing.
		 * @return std::vector<std::string>  Contents of the file read line by line, without empty lines or comments.
		 */
		std::vector<std::string> readAll(bool remove_BOM = true)
		{
			std::lock_guard lock(m_imutex);

			if (!m_ready)
			{
				ke::_internal::EngineLog::Error("FileReader is not ready to read.");
				return {};
			}
	
			std::vector<std::string> data;
	
			m_file.seekg(0, std::ios::beg);
	
			// Remove Byte Order Mark (BOM) if it exists
			if (remove_BOM)
			{
				char bom[3];
				m_file.read(bom, 3);
				if (!(bom[0] == '\xEF' && bom[1] == '\xBB' && bom[2] == '\xBF')) {
					m_file.seekg(0); // Je�li BOM nie istnieje, wracamy na pocz�tek pliku
				}
			}
			
			while (!m_file.eof())
			{
				std::string line;
				std::getline(m_file, line);
	
				if (line.empty())
					continue;
	
				// comment
				if (line.size() >= 2 && line[0] == '/' && line[1] == '/')
					continue;
	
				data.push_back(line);
			}
	
			return data;
		}
	};

} // namespace ke
