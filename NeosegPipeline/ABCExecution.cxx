#include "ABCExecution.h"

ABCExecution::ABCExecution(QString module)
    : Script(module)
{

}

void ABCExecution::update(){

    initialize();
    writeXMLFile();
    implementStop();
    implementCheckFileExistence();
    implementExecute();
    implementRun();
    writeScript();
}

void ABCExecution::initialize(){
    definePython();
    importGeneralModules();
    importXmlModules();

    defineExecutable("ABC");


    m_script += "logger = logging.getLogger('NeosegPipeline')\n\n";

    m_script += "runningProcess = None\n\n";
}

void ABCExecution::implementRun(){

    m_script += "def run():\n";

    m_script += m_indent + "signal.signal(signal.SIGINT, stop)\n";
    m_script += m_indent + "signal.signal(signal.SIGTERM, stop)\n\n";

    m_script += m_indent + "logger.info('=== ABC Execution ===')\n";

    m_script += m_indent + "logger.info('Rename generated atlas')\n";


    QString atlasdir = m_parameters->getExistingAtlas() + QString("/");
    QString atlasExt = QString(m_parameters->getAtlasFormat());
    //T1 or T2
    QString imagetype = m_parameters->getNeosegParameters()->getReferenceImage();


    QStringList inputStringList;
    QStringList outputStringList;

    inputStringList.push_back(atlasdir + QString("template") + imagetype + QString(".") + atlasExt );
    outputStringList.push_back(atlasdir + QString("template.") + atlasExt);

    inputStringList.push_back(atlasdir + QString("white.") + atlasExt);
    inputStringList.push_back(atlasdir + QString("csf.") + atlasExt);
    inputStringList.push_back(atlasdir + QString("gray.") + atlasExt);
    inputStringList.push_back(atlasdir + QString("rest.") + atlasExt);

    outputStringList.push_back(atlasdir + QString("1.") + atlasExt);
    outputStringList.push_back(atlasdir + QString("2.") + atlasExt);
    outputStringList.push_back(atlasdir + QString("3.") + atlasExt);
    outputStringList.push_back(atlasdir + QString("4.") + atlasExt);

    addRename(inputStringList, outputStringList);

    m_script+= m_indent + "args = [ABC, parameters_path]\n";
    m_script+= m_indent + "execute(args)";
}

void ABCExecution::addRename(QString s1, QString s2){
    m_script += m_indent + "if checkFileExistence('"+ s1 + "')==True:\n";
    m_script += m_indent + m_indent + "os.rename('" + s1 + "', '" + s2 + "')\n";
}

void ABCExecution::addRename(QStringList l1, QStringList l2){
    for(QStringList::iterator s1 = l1.begin(), s2 = l2.begin(); s1 != l1.end() && s2 != l2.end(); ++s1, ++s2){
        addRename(*s1, *s2);
    }
}

void ABCExecution::writeXMLFile(){

    QString XML_path = m_module_dir->filePath("parameters.xml");
    m_script += "parameters_path = \"" + XML_path + "\"\n";

    QFile* file = new::QFile(XML_path);
    file->open(QIODevice::WriteOnly);
    QXmlStreamWriter* stream = new::QXmlStreamWriter(file);
    stream->setAutoFormatting(true);

    stream->writeStartDocument();
    stream->writeDTD("<!DOCTYPE SEGMENTATION-PARAMETERS>");

    stream->writeStartElement("SEGMENTATION-PARAMETERS");


    writeABCParameters(stream);

    stream->writeEndElement();
    stream->writeEndDocument();

    file->close();

}

void ABCExecution::writeABCParameters(QXmlStreamWriter *stream){

    stream->writeTextElement(QString("SUFFIX"), m_parameters->getSuffix());

    stream->writeTextElement(QString("ATLAS-DIRECTORY"), m_parameters->getExistingAtlas());

    stream->writeTextElement(QString("ATLAS-ORIENTATION"), QString("file"));
    stream->writeTextElement(QString("ATLAS-FORMAT"), m_parameters->getAtlasFormat());
    stream->writeTextElement(QString("OUTPUT-DIRECTORY"), m_module_path);
    stream->writeTextElement(QString("OUTPUT-FORMAT"), m_parameters->getABCOutputImageFormat());

    stream->writeStartElement("IMAGE");
    stream->writeTextElement(QString("FILE"), QString(m_parameters->getT1()));
    stream->writeTextElement(QString("ORIENTATION"), QString("file"));
    stream->writeEndElement();

    stream->writeStartElement("IMAGE");
    stream->writeTextElement(QString("FILE"), QString(m_parameters->getT2()));
    stream->writeTextElement(QString("ORIENTATION"), QString("file"));
    stream->writeEndElement();

    stream->writeTextElement(QString("FILTER-ITERATIONS"), QString::number(m_parameters->getNeosegParameters()->getNumberOfIterations()));//This is not used in this filter, set to 0
    stream->writeTextElement(QString("FILTER-TIME-STEP"), QString::number(m_parameters->getNeosegParameters()->getTimeStep()));
    stream->writeTextElement(QString("FILTER-METHOD"), m_parameters->getNeosegParameters()->getFilterMethod());

    stream->writeTextElement(QString("MAX-BIAS-DEGREE"), QString::number(m_parameters->getABCMaximumDegreeBiasField()));

    std::vector<double> coeffs = m_parameters->getABCPriorsCoefficients();
    for(unsigned i = 0; i < coeffs.size(); i++){
        stream->writeTextElement(QString("PRIOR"), QString::number(coeffs[i]));
    }

    stream->writeTextElement(QString("INITIAL-DISTRIBUTION-ESTIMATOR"), m_parameters->getABCInitialDistributorEstimatorType());

    stream->writeTextElement("DO-ATLAS-WARP", QString("0"));
    stream->writeTextElement("ATLAS-WARP-FLUID-ITERATIONS", QString("0"));
    stream->writeTextElement("ATLAS-WARP-FLUID-MAX-STEP", QString("0"));
    stream->writeTextElement("ATLAS-LINEAR-MAP-TYPE", QString("rigid"));
    stream->writeTextElement("IMAGE-LINEAR-MAP-TYPE", QString("id"));

}
