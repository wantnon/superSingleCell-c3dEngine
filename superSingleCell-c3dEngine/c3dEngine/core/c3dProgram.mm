//
//  c3dProgram.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-16.
//
//

#include "c3dProgram.h"
bool Cc3dProgram::initWithFile(const string&vertShaderFilePath,const string&fragShaderFilePath){
    m_vertShaderFilePath=vertShaderFilePath;
    m_fragShaderFilePath=fragShaderFilePath;
    //create shader
    vector<string> _vertShaderFileName=splitStrInTwoByLastDot(vertShaderFilePath);
    vector<string> _fragShaderFileName=splitStrInTwoByLastDot(fragShaderFilePath);
    //创建Program
    GLuint vertShader=createShader(_vertShaderFileName[0].c_str(), _vertShaderFileName[1].c_str());
    GLuint fragShader=createShader(_fragShaderFileName[0].c_str(), _fragShaderFileName[1].c_str());
    //Calls glCreateProgram, glAttachShader, and glLinkProgram to link the vertex and fragment shaders into a complete program.
    GLuint programHandle = glCreateProgram();
    glAttachShader(programHandle, vertShader);
    glAttachShader(programHandle, fragShader);
    
    //needs to be done prior to linking
    {
        glBindAttribLocation(programHandle, ATTRIB_LOC_position_local, "position_local");
        glBindAttribLocation(programHandle, ATTRIB_LOC_texCoord, "texCoordIn");
        glBindAttribLocation(programHandle, ATTRIB_LOC_normal_local, "normal_local");
    }
    glLinkProgram(programHandle);//link过以后，通过glGet**Location得到的索引就是固定的了
    //check and see if there were any link errors
    GLint linkSuccess;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess == GL_FALSE) {
        GLchar messages[1024];
        glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
        NSString *messageString = [NSString stringWithUTF8String:messages];
        NSLog(@"%@", messageString);
        assert(false);
    }
    m_program=programHandle;
    return true;
}
GLuint Cc3dProgram::createShader(const char*_shaderName,const char*_ext)
{
    //转化成NSString
    NSString *shaderName=[[[NSString alloc] initWithUTF8String:_shaderName] autorelease];
    NSString *ext=[[[NSString alloc] initWithUTF8String:_ext] autorelease];
    //包括加载和编译
    // 0
    GLenum shaderType;
    if([ext isEqualToString:@"vert"]==YES){
        shaderType=GL_VERTEX_SHADER;
    }else if([ext isEqualToString:@"frag"]==YES){
        shaderType=GL_FRAGMENT_SHADER;
    }else{
        cout<<"无效的后缀名! "<<_ext<<endl;
        exit(0);
    }
    // 1
    NSString* shaderPath = [[NSBundle mainBundle] pathForResource:shaderName ofType:ext];
    //   cout<<[shaderPath UTF8String]<<endl;
    NSError* error;
    NSString* shaderString = [NSString stringWithContentsOfFile:shaderPath encoding:NSUTF8StringEncoding error:&error];
    if (!shaderString) {
        NSLog(@"Error loading shader: %@", error.localizedDescription);
        cout<<_shaderName<<" "<<_ext<<endl;
        assert(false);
    }
    // 2
    GLuint shaderHandle = glCreateShader(shaderType);
    // 3
    const char * shaderStringUTF8 = [shaderString UTF8String];
    int shaderStringLength = [shaderString length];
    glShaderSource(shaderHandle, 1, &shaderStringUTF8, &shaderStringLength );
    // 4
    glCompileShader(shaderHandle);
    // 5
    GLint compileSuccess;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);
    if (compileSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
        NSString *messageString = [NSString stringWithUTF8String:messages];
        NSLog(@"%@", messageString);
        exit(1);
    }
    return shaderHandle;
}

void Cc3dProgram::passUnifoValue1f(string unifoName,GLfloat v){
    glUniform1f(m_unifoMap[unifoName],v);
}
void Cc3dProgram::passUnifoValue1i(string unifoName,GLint v){
    glUniform1i(m_unifoMap[unifoName],v);
}
void Cc3dProgram::passUnifoValueMatrixNfv(string unifoName,const GLfloat *array,int arrayLen){
    assert(arrayLen==4||arrayLen==9||arrayLen==16);
    switch (arrayLen) {
        case 4:
            glUniformMatrix2fv(m_unifoMap[unifoName],1,GL_FALSE,array);
            break;
        case 9:
            glUniformMatrix3fv(m_unifoMap[unifoName],1,GL_FALSE,array);
            break;
        case 16:
            glUniformMatrix4fv(m_unifoMap[unifoName],1,GL_FALSE,array);
            break;
        default:
            assert(false);
            break;
    }
    
}
void Cc3dProgram::passUnifoValueNfv(string unifoName,const GLfloat*array,int arrayLen){
    assert(arrayLen>=1&&arrayLen<=4);
    switch (arrayLen) {
        case 1:
            glUniform1fv(m_unifoMap[unifoName],1,array);
            break;
        case 2:
            glUniform2fv(m_unifoMap[unifoName],1,array);
            break;
        case 3:
            glUniform3fv(m_unifoMap[unifoName],1,array);
            break;
        case 4:
            glUniform4fv(m_unifoMap[unifoName],1,array);
            break;
        default:
            assert(false);
            break;
    }
    
}
void Cc3dProgram::passUnifoValueNiv(string unifoName,const GLint*array,int arrayLen){
    assert(arrayLen>=1&&arrayLen<=4);
    switch (arrayLen) {
        case 1:
            glUniform1iv(m_unifoMap[unifoName],1,array);
            break;
        case 2:
            glUniform2iv(m_unifoMap[unifoName],1,array);
            break;
        case 3:
            glUniform3iv(m_unifoMap[unifoName],1,array);
            break;
        case 4:
            glUniform4iv(m_unifoMap[unifoName],1,array);
            break;
        default:
            assert(false);
            break;
    }
    
}
void Cc3dProgram::passUnifoValueMatrixNfv(string unifoName,const vector<GLfloat>&valueList){
    passUnifoValueMatrixNfv(unifoName, &valueList.front(), (int)valueList.size());
}
void Cc3dProgram::passUnifoValueNfv(string unifoName,const vector<GLfloat>&valueList){
    passUnifoValueNfv(unifoName, &valueList.front(), (int)valueList.size());
}
void Cc3dProgram::passUnifoValueNiv(string unifoName,const vector<GLint>&valueList){
    passUnifoValueNiv(unifoName, &valueList.front(), (int)valueList.size());
}

