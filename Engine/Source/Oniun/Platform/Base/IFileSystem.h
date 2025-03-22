#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/String/String.h"

namespace Oniun
{
    enum class DirectorySearch
    {
        /// Includes current directory and all subdirectories in the search.
        Recursive,
        /// Includes only the current directory.
        CurrentOnly,
    };

    enum class SpecialDirectory
    {
        Desktop,
        Documents,
        Downloads,
        Music,
        Pictures,
        Videos,
        AppData,
        LocalAppData,
        ProgramData,
        Temporary,
    };

    struct IFileSystem
    {
        static String GetCurrentDirectory() = delete;

        static bool CreateDirectory(const StringView& path) = delete;


        /// Deletes the directory and all its contents (including subdirectories).
        ///
        /// @param path The path of the directory to be deleted.
        /// @return True if the directory was deleted successfully, false otherwise.
        static bool DeleteDirectory(const StringView& path) = delete;

        static bool DirectoryExists(const StringView& path) = delete;


        /// Gets a list of files in the specified directory.
        ///
        /// @param filePaths     An array to store the paths of the files.
        /// @param path          The path of the directory to search.
        /// @param searchPattern A pattern to filter the files (e.g., "*.txt").
        /// @param searchOpt     Options for the search (e.g., recursive or non-recursive).
        /// @return True if the files were retrieved successfully, false otherwise.
        static bool GetDirectoryFiles(Array<String>& filePaths, const StringView& path, const StringView& searchPattern,
                                      DirectorySearch searchOpt) = delete;


        /// Gets a list of child directories in the specified directory.
        ///
        /// @param directories An array to store the paths of the child directories.
        /// @param path        The path of the directory to search.
        /// @return True if the child directories were retrieved successfully, false otherwise.
        static bool GetChildDirectories(Array<String>& directories, const StringView& path) = delete;


        /// Gets the path of a special directory (e.g., documents, desktop, etc.).
        ///
        /// @param type The type of special directory to get.
        /// @return The path of the special directory as a string.
        static String GetSpecialDirectoryPath(SpecialDirectory type) = delete;

        static bool DeleteFile(const StringView& path) = delete;
        static bool MoveFile(const StringView& destPath, const StringView& srcPath, bool overwriteExisting = false) = delete
        ;
        static bool CopyFile(const StringView& destPath, const StringView& srcPath, bool overwriteExisting = false) = delete
        ;

        static uint64 GetFileSize(const StringView& path) = delete;
        static bool FileExists(const StringView& path) = delete;


        /// Opens the standard dialog window prompt for the user to select a file.
        ///
        /// @param openPath The initial directory path to open the dialog in.
        /// @param filter   A string specifying the file types to display in the dialog (e.g., "Text Files\0*.txt\0").
        /// @param title    The title of the file dialog.
        /// @return The path of the selected file as a string.
        static String OpenDialog(const StringView& openPath, const StringView& filter, const StringView& title) = delete;


        /// Opens the standard dialog window prompt for the user to select multiple files.
        ///
        /// @param openPath The initial directory path to open the dialog in.
        /// @param filter   A string specifying the file types to display in the dialog (e.g., "Text Files\0*.txt\0").
        /// @param title    The title of the file dialog. Defaults to an empty string if not provided.
        /// @return An array of strings containing the paths of the selected files.
        static Array<String> OpenDialogMultiple(const StringView& openPath, const StringView& filter,
                                                const StringView& title = nullptr) = delete;


        /// Opens the standard dialog window prompt for the user to set a save location.
        ///
        /// @param openPath The initial directory path to open the dialog in.
        /// @param filter   A string specifying the file types to display in the dialog (e.g., "Text Files\0*.txt\0").
        /// @param title    The title of the file dialog.
        /// @return The path of the file to be saved as a string.
        static String SaveDialog(const StringView& openPath, const StringView& filter, const StringView& title) = delete;


        /// Opens a dialog to select a directory.
        ///
        /// @param openPath The initial directory path to open the dialog in.
        /// @param title    The title of the folder dialog.
        /// @return The path of the selected directory as a string.
        static String PickDirectoryDialog(const StringView& openPath, const StringView& title) = delete;

        /// Opens the file explorer at the specified directory path.
        ///
        /// @param openPath The directory path to open the file explorer in.
        static void OpenFileExplorer(const StringView& openPath) = delete;

        static String RelativePathToAbsolute(const StringView& path);
        static String AbsolutePathToRelative(const StringView& path);

        static String GetTemporaryFileName(const StringView& currentPath);
        static Slice<char> GetFileNameFromPath(const StringView& path);
        static Slice<char> GetDirectoryNameFromFilePath(const StringView& path);
    };
}
