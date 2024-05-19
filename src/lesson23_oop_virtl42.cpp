//============================================================================
// Author      : Sven Göthel
// Copyright   : 2022 Göthel Software e.K.
// License     : MIT
// Description : C++ Lesson 2.3 OOP (dreaded diamond problem, inheritance w/ virtual base classes, showing ctor/dtor)
//============================================================================

#include <iostream>

#include<iostream>

/**
 * test01 demos inheritance of two OO trees (wrong utilization): 
 * - exception, runtime_error, system_error: Mimiks non-virtual inheritance of std::exception
 * - ExceptionBase, RuntimeException, RuntimeSystemException: User virtual derived OO tree additionally inheriting from exception
 * 
 * Problem:
 * - runtime_error, system_error not using virtual base class 
 *   leading to multiple exception and runtime_error instances and ctor/dtor calls    
 */
namespace test01 {
    class exception {
        public:
            exception()
            {
                std::cout<< "exception::ctor" <<std::endl;            
            }
            
            virtual ~exception() {
                std::cout<< "exception::dtor" <<std::endl;                        
            }
            virtual const char* what() const noexcept {
                return "exception::what()";
            }
    };
    
    class runtime_error : public exception {
        protected:
            std::string msg0;
        public:
            runtime_error(const std::string& m)
            : exception(), msg0(m)
            {
                std::cout<< "runtime_error::ctor: '" << msg0 << "'" <<std::endl;
            }
    
            ~runtime_error() override
            {
                std::cout<< "runtime_error::dtor: '" << msg0 << "'" <<std::endl;
            }
            
            virtual const char* what() const noexcept override {
                static std::string s = "runtime_error::what("+msg0+")";
                return s.c_str();
            }
            
    };
    
    class system_error : public runtime_error {
        public:
            system_error(const std::string& m)
            : runtime_error(m)
            {
                std::cout<< "system_error::ctor: '" << msg0 << "'" <<std::endl;
            }
    
            ~system_error() override
            {
                std::cout<< "system_error::dtor: '" << msg0 << "'" <<std::endl;
            }
            virtual const char* what() const noexcept override {
                static std::string s = "system_error::what("+msg0+")";
                return s.c_str();
            }
    };
    
    class ExceptionBase : public virtual exception {
        protected:
            std::string msg1;
        public:
            ExceptionBase(const std::string& m)
            : exception(), msg1(m) 
            {
                std::cout<< "ExceptionBase::ctor '" << msg1 << "'" <<std::endl;            
            }
            
            virtual ~ExceptionBase() {
                std::cout<< "ExceptionBase::dtor '" << msg1 << "'" <<std::endl;            
            }
        
            const std::string& whole_message() const noexcept { return msg1; }
            
            virtual const char* what() const noexcept override {
                static std::string s = "ExceptionBase::what("+msg1+")";
                return s.c_str();
            }
        
    };
    
    class RuntimeException : public virtual ExceptionBase, public virtual runtime_error {
      public:
        RuntimeException(std::string const& m) noexcept
        : exception(), ExceptionBase(m), runtime_error(m) {
                std::cout<< "RuntimeException::ctor '" << msg1 << "'" <<std::endl;                    
        }
    
        ~RuntimeException() noexcept override {
                std::cout<< "RuntimeException::dtor '" << msg1 << "'" <<std::endl;                            
        }
    
        RuntimeException(const RuntimeException& o) = default;
        RuntimeException(RuntimeException&& o) = default;
        RuntimeException& operator=(const RuntimeException& o) = default;
        RuntimeException& operator=(RuntimeException&& o) = default;
        
        virtual const char* what() const noexcept override {
            static std::string s = "RuntimeException::what("+msg1+")";
            return s.c_str();
        }
    };        
    
    class RuntimeSystemException : public virtual RuntimeException, public virtual system_error {
      public:
        RuntimeSystemException(std::string const& m) noexcept
        : exception(), ExceptionBase(m), runtime_error(ExceptionBase::whole_message()), 
          RuntimeException(m), system_error(whole_message()) {
                std::cout<< "RuntimeSystemException::ctor '" << msg1 << "'" <<std::endl;                            
          }
    
        ~RuntimeSystemException() noexcept override {
                std::cout<< "RuntimeSystemException::dtor '" << msg1 << "'" <<std::endl;                                    
        }
    
        RuntimeSystemException(const RuntimeSystemException& o) = default;
        RuntimeSystemException(RuntimeSystemException&& o) = default;
        RuntimeSystemException& operator=(const RuntimeSystemException& o) = default;
        RuntimeSystemException& operator=(RuntimeSystemException&& o) = default;
        
        virtual const char* what() const noexcept override {
            static std::string s = "RuntimeSystemException::what("+msg1+")";
            return s.c_str();
        }
    };
}

/**
 * test02 demos fixed inheritance of two OO trees (fix 1): 
 * - exception, runtime_error, system_error: Uses fixed virtual inheritance
 * - ExceptionBase, RuntimeException, RuntimeSystemException: User virtual derived OO tree additionally inheriting from exception
 * 
 * Fixed:
 * - runtime_error, system_error is using virtual base class here, 
 *   hence _single_ exception and runtime_error instances and ctor/dtor calls    
 */
namespace test02 {
    class exception {
        public:
            exception()
            {
                std::cout<< "exception::ctor" <<std::endl;            
            }
            
            virtual ~exception() {
                std::cout<< "exception::dtor" <<std::endl;                        
            }
            virtual const char* what() const noexcept {
                return "exception::what()";
            }
    };
    
    class runtime_error : public virtual exception {
        protected:
            std::string msg0;
        public:
            runtime_error(const std::string& m)
            : exception(), msg0(m)
            {
                std::cout<< "runtime_error::ctor: '" << msg0 << "'" <<std::endl;
            }
    
            ~runtime_error() override
            {
                std::cout<< "runtime_error::dtor: '" << msg0 << "'" <<std::endl;
            }
            
            virtual const char* what() const noexcept override {
                static std::string s = "runtime_error::what("+msg0+")";
                return s.c_str();
            }
            
    };
    
    class system_error : public virtual runtime_error {
        public:
            system_error(const std::string& m)
            : runtime_error(m)
            {
                std::cout<< "system_error::ctor: '" << msg0 << "'" <<std::endl;
            }
    
            ~system_error() override
            {
                std::cout<< "system_error::dtor: '" << msg0 << "'" <<std::endl;
            }
            virtual const char* what() const noexcept override {
                static std::string s = "system_error::what("+msg0+")";
                return s.c_str();
            }
    };
    
    class ExceptionBase : public virtual exception {
        protected:
            std::string msg1;
        public:
            ExceptionBase(const std::string& m)
            : exception(), msg1(m) 
            {
                std::cout<< "ExceptionBase::ctor '" << msg1 << "'" <<std::endl;            
            }
            
            virtual ~ExceptionBase() {
                std::cout<< "ExceptionBase::dtor '" << msg1 << "'" <<std::endl;            
            }
        
            const std::string& whole_message() const noexcept { return msg1; }
            
            virtual const char* what() const noexcept override {
                static std::string s = "ExceptionBase::what("+msg1+")";
                return s.c_str();
            }
        
    };
    
    class RuntimeException : public virtual ExceptionBase, public virtual runtime_error {
      public:
        RuntimeException(std::string const& m) noexcept
        : exception(), ExceptionBase(m), runtime_error(m) {
                std::cout<< "RuntimeException::ctor '" << msg1 << "'" <<std::endl;                    
        }
    
        ~RuntimeException() noexcept override {
                std::cout<< "RuntimeException::dtor '" << msg1 << "'" <<std::endl;                            
        }
    
        RuntimeException(const RuntimeException& o) = default;
        RuntimeException(RuntimeException&& o) = default;
        RuntimeException& operator=(const RuntimeException& o) = default;
        RuntimeException& operator=(RuntimeException&& o) = default;
        
        virtual const char* what() const noexcept override {
            static std::string s = "RuntimeException::what("+msg1+")";
            return s.c_str();
        }
    };        
    
    class RuntimeSystemException : public virtual RuntimeException, public virtual system_error {
      public:
        RuntimeSystemException(std::string const& m) noexcept
        : exception(), ExceptionBase(m), runtime_error(ExceptionBase::whole_message()), 
          RuntimeException(m), system_error(whole_message()) {
                std::cout<< "RuntimeSystemException::ctor '" << msg1 << "'" <<std::endl;                            
          }
    
        ~RuntimeSystemException() noexcept override {
                std::cout<< "RuntimeSystemException::dtor '" << msg1 << "'" <<std::endl;                                    
        }
    
        RuntimeSystemException(const RuntimeSystemException& o) = default;
        RuntimeSystemException(RuntimeSystemException&& o) = default;
        RuntimeSystemException& operator=(const RuntimeSystemException& o) = default;
        RuntimeSystemException& operator=(RuntimeSystemException&& o) = default;
        
        virtual const char* what() const noexcept override {
            static std::string s = "RuntimeSystemException::what("+msg1+")";
            return s.c_str();
        }
    };    
}

/**
 * test03 demos inheritance of two OO trees (wrong utilization): 
 * - exception, runtime_error, system_error: Mimiks non-virtual inheritance of std::exception
 * - ExceptionBase, RuntimeException, RuntimeSystemException: User non-virtual derived OO tree additionally 
 *   selectively inheriting from exception, avoiding duplicated base classes.
 * 
 * Fixed:
 * - runtime_error, system_error not using non-virtual base class, 
 *   but carefully inheriting from these leads to _single_ exception and runtime_error instances and ctor/dtor calls
 */
namespace test03 {
    class exception {
        public:
            exception()
            {
                std::cout<< "exception::ctor" <<std::endl;            
            }
            
            virtual ~exception() {
                std::cout<< "exception::dtor" <<std::endl;                        
            }
            virtual const char* what() const noexcept {
                return "exception::what()";
            }
    };
    
    class runtime_error : public exception {
        protected:
            std::string msg0;
        public:
            runtime_error(const std::string& m)
            : exception(), msg0(m)
            {
                std::cout<< "runtime_error::ctor: '" << msg0 << "'" <<std::endl;
            }
    
            ~runtime_error() override
            {
                std::cout<< "runtime_error::dtor: '" << msg0 << "'" <<std::endl;
            }
            
            const char* what() const noexcept override {
                static std::string s = "runtime_error::what("+msg0+")";
                return s.c_str();
            }
            
    };
    
    class system_error : public runtime_error {
        public:
            system_error(const std::string& m)
            : runtime_error(m)
            {
                std::cout<< "system_error::ctor: '" << msg0 << "'" <<std::endl;
            }
    
            ~system_error() override
            {
                std::cout<< "system_error::dtor: '" << msg0 << "'" <<std::endl;
            }
            const char* what() const noexcept override {
                static std::string s = "system_error::what("+msg0+")";
                return s.c_str();
            }
    };
    
    class ExceptionBase {
        protected:
            std::string msg1;
        public:
            ExceptionBase(const std::string& m)
            : msg1(m) 
            {
                std::cout<< "ExceptionBase::ctor '" << msg1 << "'" <<std::endl;            
            }
            
            virtual ~ExceptionBase() {
                std::cout<< "ExceptionBase::dtor '" << msg1 << "'" <<std::endl;            
            }
        
            const std::string& whole_message() const noexcept { return msg1; }
            
            virtual const char* what() const noexcept {
                static std::string s = "ExceptionBase::what("+msg1+")";
                return s.c_str();
            }
        
    };
    
    class RuntimeException : public ExceptionBase {
      protected:
        RuntimeException(std::string const& m) noexcept
        : ExceptionBase(m) {
                std::cout<< "RuntimeException::ctor '" << msg1 << "'" <<std::endl;                    
        }
        
      public:   
        ~RuntimeException() noexcept override {
                std::cout<< "RuntimeException::dtor '" << msg1 << "'" <<std::endl;                            
        }
    
        RuntimeException(const RuntimeException& o) = default;
        RuntimeException(RuntimeException&& o) = default;
        RuntimeException& operator=(const RuntimeException& o) = default;
        RuntimeException& operator=(RuntimeException&& o) = default;
        
        const char* what() const noexcept override {
            static std::string s = "RuntimeException::what("+msg1+")";
            return s.c_str();
        }
    };        
    
    class RuntimeExceptionStd : public RuntimeException, public runtime_error {
      public:
        RuntimeExceptionStd(std::string const& m) noexcept
        : RuntimeException(m), runtime_error(m) {
                std::cout<< "RuntimeExceptionStd::ctor '" << msg1 << "'" <<std::endl;                    
        }
    
        ~RuntimeExceptionStd() noexcept override {
                std::cout<< "RuntimeExceptionStd::dtor '" << msg1 << "'" <<std::endl;                            
        }
    
        const char* what() const noexcept override {
            static std::string s = "RuntimeException::what("+msg1+")";
            return s.c_str();
        }
    };
            
    class RuntimeSystemException : public RuntimeException, public system_error {
      public:
        RuntimeSystemException(std::string const& m) noexcept
        : RuntimeException(m), system_error(whole_message()) {
                std::cout<< "RuntimeSystemException::ctor '" << msg1 << "'" <<std::endl;                            
          }
    
        ~RuntimeSystemException() noexcept override {
                std::cout<< "RuntimeSystemException::dtor '" << msg1 << "'" <<std::endl;                                    
        }
    
        RuntimeSystemException(const RuntimeSystemException& o) = default;
        RuntimeSystemException(RuntimeSystemException&& o) = default;
        RuntimeSystemException& operator=(const RuntimeSystemException& o) = default;
        RuntimeSystemException& operator=(RuntimeSystemException&& o) = default;
        
        const char* what() const noexcept override {
            static std::string s = "RuntimeSystemException::what("+msg1+")";
            return s.c_str();
        }
    };    
}

int main()
{
    {
        std::cout<< "Main: test01::RuntimeSystemException Create..." <<std::endl;
        test01::RuntimeSystemException rse("test01");
        std::cout<<std::endl<<std::endl;
        std::cout<< "Main: test01::RuntimeSystemException what.." <<std::endl;
        printf("XXX: %s\n", rse.what());
        std::cout<<std::endl<<std::endl;
        std::cout<< "Main: test01::RuntimeSystemException Dtor..." <<std::endl;
    }
    std::cout<<std::endl<<std::endl;
    std::cout<< "Main: test01::RuntimeSystemException Done" <<std::endl;
    
    {
        std::cout<< "Main: test02::RuntimeSystemException Create..." <<std::endl;
        test02::RuntimeSystemException rse("test02");
        std::cout<<std::endl<<std::endl;
        std::cout<< "Main: test02::RuntimeSystemException what.." <<std::endl;
        printf("XXX: %s\n", rse.what());
        std::cout<<std::endl<<std::endl;
        std::cout<< "Main: test02::RuntimeSystemException Dtor..." <<std::endl;
    }
    std::cout<<std::endl<<std::endl;
    std::cout<< "Main: test02::RuntimeSystemException Done" <<std::endl;
    
    {
        std::cout<< "Main: test03::RuntimeSystemException Create..." <<std::endl;
        test03::RuntimeSystemException rse("test03");
        std::cout<<std::endl<<std::endl;
        std::cout<< "Main: test03::RuntimeSystemException what.." <<std::endl;
        printf("XXX: %s\n", rse.what());
        std::cout<<std::endl<<std::endl;
        std::cout<< "Main: test03::RuntimeSystemException Dtor..." <<std::endl;
    }
    std::cout<<std::endl<<std::endl;
    std::cout<< "Main: test03::RuntimeSystemException Done" <<std::endl;
}
