#ifndef UTILITIES_H
#define UTILITIES_H

#include <QObject>
#include <QTableWidget>

class Utilities
{
public:
    Utilities();

    ///
    /// \brief SaveData
    /// \param data
    /// \param name
    /// \param file_path
    /// \return
    ///
    static bool SaveData( const QStringList &data, const QString &name, const QString &file_path );

    ///
    /// \brief ReadFile
    /// \param file_path
    /// \return
    ///
    static bool ReadFile( const QString &file_path );

    ///
    /// \brief LoadData
    /// \param data
    ///
    static void LoadData( const QStringList &data );

    ///
    /// \brief SendData
    /// \param data
    /// \return
    ///
    static bool SendData( const QString &name, const QStringList &data );

    ///
    /// \brief GetData
    /// \param table
    /// \return
    ///
    static QStringList GetData( const QTableWidget * table );

    static bool CheckNumber( const QString &number );

    static bool CheckName( const QString &name );
};

#endif // UTILITIES_H
