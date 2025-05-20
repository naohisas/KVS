/*****************************************************************************/
/**
 *  @file   MainWindow.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/qt/Qt>
#include <kvs/qt/Screen>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/RayCastingRenderer>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QDir>


/*===========================================================================*/
/**
 *  @brief  MainWindow class.
 */
/*===========================================================================*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    kvs::qt::Screen* m_screen = nullptr; ///< pointer to the screen
    QMenu* m_file_menu = nullptr; ///< file menu
    QAction* m_open_action = nullptr; ///< action for file-open

public:
    MainWindow( kvs::qt::Application* app )
    {
        m_screen = new kvs::qt::Screen( app );
        QMainWindow::setCentralWidget( m_screen );

        m_open_action = new QAction( this );
        m_open_action->setText( tr("&Open...") );
        m_open_action->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_O ) );
        m_open_action->setShortcutContext( Qt::ApplicationShortcut );
        QObject::connect( m_open_action, SIGNAL( triggered() ), this, SLOT( open() ) );

        m_file_menu = menuBar()->addMenu( tr("&File") );
        m_file_menu->addAction( m_open_action );

        m_screen->show();
    }

    ~MainWindow()
    {
        delete m_screen;
        delete m_open_action;
        delete m_file_menu;
    }

private:
    Q_SLOT void open()
    {
        auto* dialog = new QFileDialog( this, "Open file ...", QDir::homePath() );
        if ( dialog->exec() )
        {
            using Importer = kvs::StructuredVolumeImporter;
            using Object = kvs::StructuredVolumeObject;
            using Renderer = kvs::glsl::RayCastingRenderer;

            auto object_name = std::string( "Object" );
            if ( m_screen->scene()->hasObject( object_name ) )
            {
                m_screen->scene()->removeObject( object_name );
            }

            auto filenames = dialog->selectedFiles();
            auto filename = filenames.at(0);
            auto* object = new Importer( filename.toStdString() );
            auto* renderer = new Renderer();

            object->setName( object_name );
            m_screen->registerObject( object, renderer );
            m_screen->redraw();
        }
    }
};
